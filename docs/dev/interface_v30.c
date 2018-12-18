#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "common_mod.h"
#include "common_func.h"
#include "interface_v30.h"
#include "MK1_var_def.h"
#include "m_strat_func.h"
#include "MK1_func.h"




//----------------------------------------------------
// subroutine st_feed_localquote(lstStructIn, liNoTSignal, lastStructTSignal)
// description: internal info function
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_localquote_(struct cStruct_local_quote *lstStructIn, int *liNoTSignal,
	struct signal_t *lastStructTSignal)
{
	*liNoTSignal = 0;
}

//----------------------------------------------------
// subroutine st_feed_sig_resp(s_cStructIn, liNoTSignal, lastStructTSignal)
// description: initialization function
// input: liFileNumber
//        lcFileName
//----------------------------------------------------
//void st_feed_sig_resp_(struct signal_resp_t *lstStructOrderStatusIn,
//	struct symbol_pos_t *lstStructPositionIn, struct pending_order_t
//	*lstStructPendingPOSIn, int *liNoTSignal, struct signal_t *lastStructTSignal)
//{
//#ifdef WRITE_SYSLOG_
//    syslog(LOG_ERR, "do st_feed_sig_resp_\n");
//#endif // WRITE_SYSLOG_
//
//	*liNoTSignal = 0;
//	giNoTradeItemEachRun = 0;
//
//    gstLocTradeInfo.iPosD = lstStructPositionIn->long_volume;
//	gstLocTradeInfo.iPosK = lstStructPositionIn->short_volume;
//
//	s_update_my_order_info(lstStructOrderStatusIn);
//
//}

void st_feed_sig_resp_(struct signal_resp_t* lstStructOrderStatusIn, struct symbol_pos_t *lstStructPositionIn,
    int *sig_cnt, struct signal_t* sigs,struct strat_out_log *outlog)
{
    *sig_cnt = 0;
	giNoTradeItemEachRun = 0;

    gstLocTradeInfo.iPosD = lstStructPositionIn->long_volume;
	gstLocTradeInfo.iPosK = lstStructPositionIn->short_volume;

	s_update_my_order_info(lstStructOrderStatusIn);
	memset(outlog, 0, sizeof(struct strat_out_log));
}

//----------------------------------------------------
// subroutine st_feed_position(lstStructPositionIn, liNoTSignal, lastStructTSignal)
// description: get position info
//----------------------------------------------------
void st_feed_position_(struct symbol_pos_t *lstStructPositionIn, int *liNoTSignal,
	struct signal_t *lastStructTSignal)
{
	*liNoTSignal = 0;
}

//----------------------------------------------------
// subroutine st_feed_init_position(lstStructPositionIn, liNoTSignal, lastStructTSignal)
// description: initialization position function
//----------------------------------------------------
//void st_feed_init_position_(struct strategy_init_pos_t *lstStructPositionIn, int *liNoTSignal,
//	struct signal_t *lastStructTSignal)
//{
//#ifdef WRITE_SYSLOG_
//    syslog(LOG_ERR, "do st_feed_init_position_\n");
//#endif // WRITE_SYSLOG_
//    int i;
//    //struct symbol_pos_t tmp;
//
//	*liNoTSignal = 0;
//
//	if(lstStructPositionIn->today_pos.count == 0)
//    {
//        gstLocTradeInfo.iPosD = 0;
//        gstLocTradeInfo.iPosK = 0;
//    }
//    else
//    {
//		// added by wangyig: constraint:the strategy only trades a contract
//		for(i=0; i<lstStructPositionIn->today_pos.count; i++){
//			//symbol_pos_t &tmp = lstStructPositionIn->today_pos.pos[i];
//			if(0 == strcmp(lstStructPositionIn->today_pos.pos[i].ticker,gcTicker)){
//				gstLocTradeInfo.iPosD = lstStructPositionIn->today_pos.pos[i].long_volume;
//				gstLocTradeInfo.iPosK = lstStructPositionIn->today_pos.pos[i].short_volume;
//				gstLocTradeInfo.iYestD = gstLocTradeInfo.iPosD;
//				gstLocTradeInfo.iYestK = gstLocTradeInfo.iPosK;
//				break;
//			}
//		}
//
//    }
//
//
//}

void st_feed_init_position_(struct strategy_init_pos_t *lstStructPositionIn,struct strat_out_log *outlog)
{
    int i;

	if(lstStructPositionIn->cur_pos.symbol_cnt == 0)
    {
        gstLocTradeInfo.iPosD = 0;
        gstLocTradeInfo.iPosK = 0;
    }
    else
    {
		// added by wangyig: constraint:the strategy only trades a contract
		for(i=0; i<lstStructPositionIn->cur_pos.symbol_cnt; i++){
			if(0 == strcmp(lstStructPositionIn->cur_pos.pos[i].ticker,gcTicker)){
				gstLocTradeInfo.iPosD = lstStructPositionIn->cur_pos.pos[i].long_volume;
				gstLocTradeInfo.iPosK = lstStructPositionIn->cur_pos.pos[i].short_volume;
				break;
			}
		}

    }
    memset(outlog, 0, sizeof(struct strat_out_log));
}

//----------------------------------------------------
// subroutine st_feed_marketinfo(s_cStructIn, liNoTSignal, lastStructTSignal)
// description: initialization function
// input: liFileNumber
//        lcFileName
//----------------------------------------------------
void st_init_(struct st_config_t *lstStructInitConfig, int *liRetCode,struct strat_out_log *outlog)
{
    time_t cur_time;
    char sztmp[64];

	// trade contract ticker is lstStructInitConfig->ticker_detail%ticker[0]
	*liRetCode = 1;
	strncpy(gcTicker, lstStructInitConfig->ticker_detail[0].ticker, 7);
	giStID = lstStructInitConfig->strategy_id;
	// added by wangyig: the following should be wrong
	giMaxPos = lstStructInitConfig->ticker_detail[0].max_pos;

	cur_time = time(0);
    strftime(sztmp, sizeof(char)*64, "%Y%m%d",localtime(&cur_time));
    giCurDate = atoi(sztmp);

    memcpy(gszEVname,lstStructInitConfig->extvariable_name, sizeof(char)*256);

	mysub0((lstStructInitConfig->ticker_detail[0].ticker_log_id),
		lstStructInitConfig->ticker_detail[0].ticker_log_name);

	*liRetCode = 0;

	memset(outlog, 0, sizeof(struct strat_out_log));

}

//----------------------------------------------------
// subroutine st_destroy()
// description: program quit
//----------------------------------------------------
void st_destroy_()
{
    s_destroy_MK1_parameters();
    fprintf(pfDayLogFile, "do st_destroy_\n");
}

//----------------------------------------------------
// subroutine st_idle(liNoTSignal, lastStructTSignal)
// description: st_idle
//----------------------------------------------------
void st_idle_(int *liNoTSignal, struct signal_t *lastStructTSignal)
{
	*liNoTSignal = 0;
}


//----------------------------------------------------
// subroutine st_feed_quote(lstStructQuote, liNoTSignal, lastStructTSignal)
// description: st_feed_quote
//----------------------------------------------------
void st_feed_quote_(struct cStruct_quote_data *lstStructQuote, int *liNoTSignal,
	struct signal_t *lastStructTSignal)
{
	*liNoTSignal = 0;
};

//----------------------------------------------------
// subroutine st_feed_marketinfo_0(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for CFFE; 0 cfex_deep_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_0_(struct cfex_deep_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{

	// init  global parameters
	if (giInitFlg == 0)
	{
		gcExch = cCFFE;
		giTickNo = 0;
	}

	// load data from lstStructIn to gstCurrIn
	if (giRunModeFlg == 1)
	{
		gstcStructCFFEIn = lstStructIn;
		s_read_fut_struct(giTickNo);
	}
	memcpy(&gstRecCurrIn, &gstCurrIn, sizeof(struct in_data));

	s_MK1_run_each_record(lastStructTSignal, liNoTSignal, outlog);
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_1(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for DCE table 1; 1 dce_my_best_deep_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_1_(struct cStruct_dl_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	// init  global parameters
	if (giInitFlg == 0)
	{
		gcExch = cDCE;
		giTickNo = 0;
	}

	 //giInternalQuoteFlg = 1;
	// load data from lstStructIn to gstCurrIn
	if (giRunModeFlg == 1)
	{
		gstcStructDCEIn = lstStructIn;
		s_read_fut_struct(giTickNo);
	}
	memcpy(&gstRecCurrIn, &gstCurrIn, sizeof(struct in_data));

	s_MK1_run_each_record(lastStructTSignal, liNoTSignal, outlog);
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_2(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for DCE table 2; 2 dce_my_march_pri_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_2_(struct cStruct_dl_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
#ifdef WRITE_SYSLOG_
    syslog(LOG_ERR, "call st_feed_marketinfo_2_\n");
#endif // WRITE_SYSLOG_

	*liNoTSignal = 0;
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_3(lstcStructDLOrderStatIn, liNoTSignal, lastStructTSignal)
// description: market info function for DCE table 3; 3 dce_my_ord_stat_quote
// input: lstcStructDLOrderStatIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_3_(struct cStruct_dl_orderstat *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	s_read_cstruct_dl_orderstat(lstStructIn, &gstCurrIn);
	*liNoTSignal = 0;
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_4(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for DCE table 4; 4 dce_my_rt_pri_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_4_(struct cStruct_dl_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	*liNoTSignal = 0;
}


//----------------------------------------------------
// subroutine st_feed_marketinfo_5(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for DCE table 5; 5 dce_my_ten_entr_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_5_(struct cStruct_dl_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
#ifdef WRITE_SYSLOG_
    syslog(LOG_ERR, "call st_feed_marketinfo_5_\n");
#endif // WRITE_SYSLOG_

	*liNoTSignal = 0;
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_6(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for SHFE;  6 shfe_my_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_6_(struct shfe_my_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	// init  global parameters
	if (giInitFlg == 0)
	{
		gcExch = cSHFE;
		giTickNo = 0;
	}

	 //giInternalQuoteFlg = 1;
	// load data from lstStructIn to gstCurrIn
	if (giRunModeFlg == 1)
	{
		gstcStructSHFEIn = lstStructIn;
		s_read_fut_struct(giTickNo);
	}
	memcpy(&gstRecCurrIn, &gstCurrIn, sizeof(struct in_data));

	s_MK1_run_each_record(lastStructTSignal, liNoTSignal, outlog);
}

//----------------------------------------------------
// subroutine st_feed_marketinfo_7(lstStructIn, liNoTSignal, lastStructTSignal)
// description: market info function for CZCE;  7 czce_my_deep_quote
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_8_(struct czce_my_deep_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	// init  global parameters
	if (giInitFlg == 0)
	{
		gcExch = cCZCE;
		giTickNo = 0;
	}

	 //giInternalQuoteFlg = 1;
	// load data from lstStructIn to gstCurrIn
	if (giRunModeFlg == 1)
	{
		gstcStructCZCEIn = lstStructIn;
		s_read_fut_struct(giTickNo);
	}
	memcpy(&gstRecCurrIn, &gstCurrIn, sizeof(struct in_data));

	s_MK1_run_each_record(lastStructTSignal, liNoTSignal, outlog);
}


//----------------------------------------------------
// subroutine st_feed_marketinfo_11(lstStructIn, liNoTSignal, lastStructTSignal)
// description: internal info function; 11 : internal quotation
// input: lstStructIn
// output:   liNoTSignal, lastStructTSignal
//----------------------------------------------------
void st_feed_marketinfo_11_(struct cStruct_local_quote *lstStructIn,
	int *liNoTSignal, struct signal_t *lastStructTSignal,struct strat_out_log *outlog)
{
	*liNoTSignal = 0;
}

