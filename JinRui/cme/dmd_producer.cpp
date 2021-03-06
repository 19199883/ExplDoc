#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <functional>   // std::bind
#include "dmd_producer.h"
#include "quote_cmn_utility.h"
#include <tinyxml.h>
#include <tinystr.h>

using namespace std::placeholders;
using namespace std;

depthMarketData* DMDProducerHelper::GetLastDataImp(const char *contract, int32_t last_index,
	depthMarketData *buffer, int32_t buffer_size,int32_t traverse_count) 
{
	depthMarketData* data = NULL;

	// 全息行情需要一档行情时，从缓存最新位置向前查找13个位置（假设有13个主力合约），找到即停
	int i = 0;
	for(; i<traverse_count; i++){
		int data_index = last_index - i;
		if(data_index < 0){
			data_index = data_index + buffer_size;
		}

		depthMarketData &tmp = buffer[data_index];
		if(strcmp(contract, tmp.name)==0){
			data = &tmp; 
			break;
		}
	}

	return data;
}

DMDProducer::DMDProducer(struct vrt_queue  *queue) : module_name_("DMDProducer")
{
	data_cursor_ = DMD_BUFFER_SIZE - 1;
	ended_ = false;
    api_ = NULL;
	clog_warning("[%s] DMD_BUFFER_SIZE:%d;",module_name_,DMD_BUFFER_SIZE);

	ParseConfig();

	// init dominant contracts
	memset(dominant_contracts_, 0, sizeof(dominant_contracts_));
	contract_count_ = LoadDominantContracts(config_.contracts_file, dominant_contracts_);
	max_traverse_count_ = contract_count_ * 4;

#ifdef PERSISTENCE_ENABLED 
    p_depthMarketData_save_ = new QuoteDataSave<depthMarketData>("depthMarketData", DEPTHMARKETDATA_QUOTE_TYPE);
#endif

	memset(&md_buffer_, 0, sizeof(md_buffer_));
	InitMDApi();

	this->producer_ = vrt_producer_new("dmd_producer", 1, queue);
	clog_warning("[%s] yield:%s", module_name_, config_.yield); 
	if(strcmp(config_.yield, "threaded") == 0){
		this->producer_ ->yield	= vrt_yield_strategy_threaded();
	}else if(strcmp(config_.yield, "spin") == 0){
		this->producer_ ->yield	= vrt_yield_strategy_spin_wait();
	}else if(strcmp(config_.yield, "hybrid") == 0){
		this->producer_ ->yield	 = vrt_yield_strategy_hybrid();
	}
}

void DMDProducer::ParseConfig()
{
	TiXmlDocument config = TiXmlDocument("x-trader.config");
    config.LoadFile();
    TiXmlElement *RootElement = config.RootElement();    

	// yield strategy
    TiXmlElement *disruptor_node = RootElement->FirstChildElement("Disruptor");
	if (disruptor_node != NULL){
		strcpy(config_.yield, disruptor_node->Attribute("yield"));
	} else { clog_error("[%s] x-shmd.config error: Disruptor node missing.", module_name_); }

    TiXmlElement *l1md_node = RootElement->FirstChildElement("depthMarketData");
	if (l1md_node != NULL){
		strcpy(config_.mcIp, l1md_node->Attribute("mcIp"));
		int mcPort = 0;
		 l1md_node->QueryIntAttribute("mcPort", &mcPort);
		this->config_.mcPort = mcPort;
	} else{
		clog_error("[%s] x-trader.config error: depthMarkerData node missing.", module_name_); 
	}
	
	// contracts file
    TiXmlElement *contracts_file_node = RootElement->FirstChildElement("Subscription");
	if (contracts_file_node != NULL){
		strcpy(config_.contracts_file, contracts_file_node->Attribute("contracts"));
	} else { clog_error("[%s] x-shmd.config error: Subscription node missing.", module_name_); }
}

DMDProducer::~DMDProducer()
{
#ifdef PERSISTENCE_ENABLED 
    if (p_depthMarketData_save_) delete p_depthMarketData_save_;
#endif
}


int32_t DMDProducer::Push(const depthMarketData& md){
	dmd_cursor_++;
	if (dmd_cursor_ % DMD_BUFFER_SIZE == 0){
		dmd_cursor_ = 0;
	}
	md_buffer_[dmd_cursor_] = md;
	return dmd_cursor_;
}

depthMarketData* DMDProducer::GetData(int32_t index)
{
	return &md_buffer_[index];
}

// lic
depthMarketData* DMDProducer::GetLastDataForIllegaluser(const char *contract)
{
	depthMarketData* data = DMDProducerHelper::GetLastDataImp(
		contract,0,md_buffer_,DMD_BUFFER_SIZE,DMD_BUFFER_SIZE);
	return data;
}

depthMarketData* DMDProducer::GetLastData(const char *contract, int32_t last_index)
{
	depthMarketData* data = DMDProducerHelper::GetLastDataImp(
		contract,last_index,md_buffer_,DMD_BUFFER_SIZE,max_traverse_count_);
	return data;
}

bool DMDProducer::IsDominant(const char *contract)
{
#ifdef PERSISTENCE_ENABLED 
	// 持久化行情时，需要记录所有合约
	clog_warning("[%s] return TRUE in IsDominant.",module_name_);
	return true;
#else
	return IsDominantImp(contract, dominant_contracts_, contract_count_);
#endif
}

void DMDProducer::ToString(depthMarketData &data)
{
	clog_warning("[%s] depthMarketData\n"
		"name:%s \n"
		"transactTime:%lu \n"
		"bid1(price:%f, size:%d, numberOfOrders:%d)\n"
		"bid2(price:%f, size:%d, numberOfOrders:%d)\n"
		"bid3(price:%f, size:%d, numberOfOrders:%d)\n"
		"bid9(price:%f, size:%d, numberOfOrders:%d)\n"
		"bid10(price:%f, size:%d, numberOfOrders:%d)\n"
		"ask1(price:%f, size:%d, numberOfOrders:%d)\n"
		"ask2(price:%f, size:%d, numberOfOrders:%d)\n"
		"ask3(price:%f, size:%d, numberOfOrders:%d)\n"
		"ask9(price:%f, size:%d, numberOfOrders:%d)\n"
		"ask10(price:%f, size:%d, numberOfOrders:%d)\n",			
		module_name_,
		data.name,
		data.transactTime,
		data.bid[0].price, data.bid[0].size, data.bid[0].numberOfOrders,
		data.bid[1].price, data.bid[1].size, data.bid[1].numberOfOrders,
		data.bid[2].price, data.bid[2].size, data.bid[2].numberOfOrders,
		data.bid[8].price, data.bid[8].size, data.bid[8].numberOfOrders,
		data.bid[9].price, data.bid[9].size, data.bid[9].numberOfOrders,		
		data.ask[0].price, data.ask[0].size, data.ask[0].numberOfOrders,
		data.ask[1].price, data.ask[1].size, data.ask[1].numberOfOrders,
		data.ask[2].price, data.ask[2].size, data.ask[2].numberOfOrders,
		data.ask[8].price, data.ask[8].size, data.ask[8].numberOfOrders,
		data.ask[9].price, data.ask[9].size, data.ask[9].numberOfOrders
		);
}

void DMDProducer::InitMDApi()
{
    api_ = CMdclientApi::Create(this,config_.mcPort,config_.mcIp);
	clog_warning("[%s] CMdclientApi ip:%s, port:%d", module_name_, config_.mcIp,config_.mcPort);

	std::ifstream is;
	is.open (config_.contracts_file);
	string contrs = "";
	if (is) {
		getline(is, contrs);
		contrs += " ";
		size_t start_pos = 0;
		size_t end_pos = 0;
		string contr = "";
		while ((end_pos=contrs.find(" ",start_pos)) != string::npos){
			contr = contrs.substr (start_pos, end_pos-start_pos);
			api_->Subscribe((char*)contr.c_str());
			clog_warning("[%s] CMdclientApi subscribe:%s",module_name_,contr.c_str());
			start_pos = end_pos + 1;
		}
	}else { clog_error("[%s] CMdclientApi can't open: %s", module_name_, config_.contracts_file); }

    int err = api_->Start();
	clog_warning("[%s] CMdclientApi start: %d", module_name_, err);
}


void DMDProducer::OnRtnDepthMarketData(depthMarketData *data)
{
	if (ended_) return;

	// 抛弃非主力合约
	if(!(IsDominant(data->InstrumentID))) return;

	RalaceInvalidValue_Femas(*data);
	
	// debug
	// ToString(*data);

	//clog_info("[%s] OnRtnDepthMarketData InstrumentID:%s,UpdateTime:%s,UpdateMillisec:%d",
	//	module_name_,data->InstrumentID,data->UpdateTime,data->UpdateMillisec);

	struct vrt_value  *vvalue;
	struct vrt_hybrid_value  *ivalue;
	vrt_producer_claim(producer_, &vvalue);
	ivalue = cork_container_of(vvalue, struct vrt_hybrid_value,parent);
	ivalue->index = Push(*data);
	ivalue->data = L1_MD;
	vrt_producer_publish(producer_);

#ifdef PERSISTENCE_ENABLED 
    timeval t;
    gettimeofday(&t, NULL);
    p_level1_save_->OnQuoteData(t.tv_sec * 1000000 + t.tv_usec, data);
#endif
}

void DMDProducer::RalaceInvalidValue_Femas(depthMarketData &d)
{
    d.Turnover = InvalidToZeroD(d.Turnover);
    d.LastPrice = InvalidToZeroD(d.LastPrice);
    d.UpperLimitPrice = InvalidToZeroD(d.UpperLimitPrice);
    d.LowerLimitPrice = InvalidToZeroD(d.LowerLimitPrice);
    d.HighestPrice = InvalidToZeroD(d.HighestPrice);
    d.LowestPrice = InvalidToZeroD(d.LowestPrice);
    d.OpenPrice = InvalidToZeroD(d.OpenPrice);
    d.ClosePrice = InvalidToZeroD(d.ClosePrice);
    d.PreClosePrice = InvalidToZeroD(d.PreClosePrice);
    d.OpenInterest = InvalidToZeroD(d.OpenInterest);
    d.PreOpenInterest = InvalidToZeroD(d.PreOpenInterest);

    d.BidPrice1 = InvalidToZeroD(d.BidPrice1);
    d.BidPrice2 = InvalidToZeroD(d.BidPrice2);
    d.BidPrice3 = InvalidToZeroD(d.BidPrice3);
    d.BidPrice4 = InvalidToZeroD(d.BidPrice4);
    d.BidPrice5 = InvalidToZeroD(d.BidPrice5);

	d.AskPrice1 = InvalidToZeroD(d.AskPrice1);
    d.AskPrice2 = InvalidToZeroD(d.AskPrice2);
    d.AskPrice3 = InvalidToZeroD(d.AskPrice3);
    d.AskPrice4 = InvalidToZeroD(d.AskPrice4);
    d.AskPrice5 = InvalidToZeroD(d.AskPrice5);

	d.SettlementPrice = InvalidToZeroD(d.SettlementPrice);
	d.PreSettlementPrice = InvalidToZeroD(d.PreSettlementPrice);

    d.PreDelta = InvalidToZeroD(d.PreDelta);
    d.CurrDelta = InvalidToZeroD(d.CurrDelta);
}


void DMDProducer::End()
{
	if(!ended_){
		ended_ = true;

		if (api_) {
			int err = api_->Stop();
			clog_warning("[%s] CMdclientApi stop: %d", module_name_, err);
			api_ = NULL;
		}

		vrt_producer_eof(producer_);
		clog_warning("[%s] End exit", module_name_);
	}
	fflush (Log::fp);
}


