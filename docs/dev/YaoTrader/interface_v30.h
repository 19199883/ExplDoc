#ifndef INTERFACE_V30_H
#define INTERFACE_V30_H

#include<string>
#include<vector>

using namespace std;

// add by.alex
struct future_quote
{
	int feed_type;                        /*int    行情源 CTP为12 参见交易所FeedType对应关系*/
	char symbol[48];                      /*str    合约代码: 如'm1801','SR801','Au(T+D)'*/
	char exchange;                        /*Exchanges    交易所代码:如'0'代表深交所,'A'代表上期所 见my.sdp.api.Exchange中定义, 参考： Exchange*/
	int int_time;                         /*integer    交易所行情时间(HHMMssmmm) 0点-3点的数据 +24hrs*/
	float pre_close_px;                   /*float    昨收盘价*/
	float pre_settle_px;                  /*float    昨结算价*/
	double pre_open_interest;             /*double    昨市场总持仓量*/
	double open_interest;                 /*double    市场总持仓量*/
	float open_px;                        /*float	开盘价*/
	float high_px;         	              /*float	最高价*/
	float low_px;	                      /*float	最低价*/
	float avg_px;	                      /*float	平均价*/
	float last_px;        	              /*float    最新价*/
	float bp_array[5];                    /*tuple	多档买价*/
	float ap_array[5];	                  /*tuple	多档卖价*/
	int bv_array[5];	                  /*tuple	多档买量*/
	int av_array[5];                      /*tuple	多档卖量*/
	int total_vol; 	                      /*int    总成交量*/
	float total_notional;	              /*float    总成交额*/
	float upper_limit_px;	              /*float	涨停价*/
	float lower_limit_px;	              /*float	跌停价*/
	float close_px;	                      /*float	收盘价  只在日盘收盘后的几笔行情有效，其余时间为0*/
	float settle_px;	                  /*float	结算价  只在日盘收盘后的几笔行情有效，其余时间为0*/
	int implied_bid_size[5];	          /*tuple	推导买量*/
	int implied_ask_size[5];              /*tuple	推导卖量*/
	int total_buy_ordsize;	              /*int	总买量 DCE OrderStat quote*/
	int	total_sell_ordsize;	              /*int	总卖量 DCE OrderStat quote*/
	float weighted_buy_px;	              /*float	平均买价 DCE OrderStat quote*/
	float weighted_sell_px;	              /*float	平均卖价 DCE OrderStat quote*/
};

struct	symbol_pos_t
{
	char	ticker[64];
	int	long_volume;
	double	long_price;
	int	short_volume;
	double	short_price;
	int	pos_change; //1: pos changed; 0: position not change

	unsigned char exchg_code;
};

struct position_t
{
	int	symbol_cnt;
	struct symbol_pos_t	s_pos[100];
};


struct	strategy_init_pos_t
{
	char st_name[64];
	struct position_t _cur_pos;
	struct position_t _yesterday_pos;
};

enum exchange_names
{
	// SHANGHAI FUTURES EXCHANGE
	XSGE = 'A',

	// CHINA FINANCIAL FUTURES EXCHANGE
	CCFX = 'G',

	// DALIAN COMMODITY EXCHANGE
	XDCE = 'B',

	// ZHENGZHOU COMMODITY EXCHANGE
	XZCE = 'C',

	// SHENZHEN STOCK EXCHANGE
	XSHE = '0',

	// SHANGHAI STOCK EXCHANGE
	XSHG = '1',
	XHKE = '2',

	EXCHG_SGX = 'S',

	undefined = 'u'

};

struct	symbol_t
{
	/*exchange code,the detail is the following:
	* 'A': SHANGHAI FUTURES EXCHANGE
	* 'G': CHINA FINANCIAL FUTURES EXCHANGE
	* 'B': DALIAN COMMODITY EXCHANGE
	* 'C': ZHENGZHOU COMMODITY EXCHANGE
	* '0': SHENZHEN STOCK EXCHANGE
	* '1': SHANGHAI STOCK EXCHANGE
	*/
	enum exchange_names	exch;
	char	ticker[64];//contract name
	int	max_pos;
	int	ticker_log_id;
	char ticker_log_name[256];
};

struct st_config_t
{

	int	strategy_id;    //strategy id
	char	strategy_name[64];  //strategy name;

	int	log_id;     // model log file id in Fortran
	char	log_name[256];  /* model log file path,not including file name */

	int	intvariable_id;   /* the file id which is used to record intermediate variables by model. */
	char	intvariable_name[256];  /* the file name,including path,which is
									used to record intermediate variables by model.*/
	int	extvariable_id;     /* the file id which is used to record external variables by model.*/
	char	extvariable_name[256]; /* the file name,including path,which is
								   used to record external variables by model. */
	//int	order_lmt_per_second;      /* order limit per second */
	//int	order_cancel_lmt_per_day;   /* order cancel limit per day*/

	int	ticker_count;          /* number of contracts */
	struct symbol_t	ticker_detail[5]; /* fixed length array and specific contract number
									  is specified by ticker_count*/
	//int log_flags;
	//int contract_info_count;
	//struct ContractInfo_t contracts[2000];

	// TODO: yao
	int trading_date;
	/*
	* 1:夜盘交易
	* 0：日盘交易
	*/
	int day_night;
};


//alex.add from wy
enum if_sig_state_t {
	//SIG_STATUS_SUCCESS = 0,

	///* 报单委托成功 */
	//SIG_STATUS_ENTRUSTED,

	//SIG_STATUS_PARTED,
	//SIG_STATUS_CANCEL,
	//SIG_STATUS_REJECTED,

	SIG_STATUS_INIT = -1,
	SIG_STATUS_SUCCESS = 0,
	SIG_STATUS_ENTRUSTED = 1,
	SIG_STATUS_PARTED = 2,
	SIG_STATUS_CANCEL = 3,
	SIG_STATUS_REJECTED = 4,
	SIG_STATUS_CANCEL_REJECTED = 5,
	SIG_STATUS_INTERREJECTED = 6,
	SIG_UNDEFINED_STATUS = 7
};

namespace py2c
{   
	//alex.add from wy
    enum alloc_position_effect_t{
	//open = 1,
	//close = 2,
	//close_yes = 3,

	open = 0,
	close = 1,
	close_tod = 2,
	close_yes = 3
	};
}

//alex.add from wy
enum  signal_act_t
{
	//buy = 1,
	//sell = 2,
	//cancel = 4,

	buy = 0,
	sell = 1,
	cancel = 2,
};


struct signal_resp_t{
	unsigned long sig_id; 				/* 信号id */
	char symbol[64]; 					/* 合约号 */
	unsigned short sig_act; 			/* action */
	int order_volume; 					/* 此信号的总委托量 */
	double order_price; 				/* 此信号的委托价格 */
	double exec_price; 					/* 当前成交价格 */
	int exec_volume; 					/* 当前成交量 */
	double aver_price; 					/* 平均成交价格 */
	int acc_volume; 					/* 累计成交量 */
	int status; 						/* 成交状态 */
	int ack; 							/* 提交数量 */
	int killed; 						/* 撤销数量 */
	int rejected; 						/* 拒绝数量 */
	int error_no; 						/* 错误号 */
	// TODO: yao
	unsigned short sig_openclose; /* 开平标识，1表⽰示开；2表⽰示平;3 close and then open */
};

/*
* if the field "instr" is equal to  QUOTE, the value of open_volume field is set to
* the buy_volume field of quote order and the value close_volume field is set to the sell_volume field of quote order
*/
struct signal_t { //alex. change class to struct

	/* 信号id */
	unsigned long sig_id;

	/*
	* the signal id to cancel.
	* it is valid only if sig_act==cancel
	*/
	unsigned long orig_sig_id;

	/* 策略id */
	int st_id;

	/* exchange code,the detail is the following:
	* 'A': SHANGHAI FUTURES EXCHANGE，上海期货交易所
	* 'G': CHINA FINANCIAL FUTURES EXCHANGE,中国金融期货交易所
	* 'B': DALIAN COMMODITY EXCHANGE，大连商品交易所
	* 'C': ZHENGZHOU COMMODITY EXCHANGE， 郑州商品交易所
	* '0': SHENZHEN STOCK EXCHANGE，深圳股票交易所
	* '1': SHANGHAI STOCK EXCHANGE，上海股票交易所
	*/
	exchange_names exchange;

	/* 合约号 */
	char symbol[64];
	/* 数量 */
	int open_volume;
	/* 价格 */
	double buy_price;
	/* 数量 */
	int close_volume;
	/* 价格 */
	double sell_price;
	/* 买/卖/取消 */
	unsigned short int sig_act;
	unsigned short sig_openclose; /* 开平标识，1表⽰示开；2表⽰示平;3 close and then open */
	/* FAK/FOK/MARKET/ROD/QUOTE */
	unsigned short int instr;
};



void st_init_(struct st_config_t *lstStructInitConfig, int *liRetCode, struct strat_out_log *outlog);
void st_feed_init_position_(struct strategy_init_pos_t *lstStructPositionIn, struct strat_out_log *outlog);

void st_feed_future_quote_(struct future_quote *lstStructIn, int *liNoTSignal, struct signal_t *lastStructTSignal, struct strat_out_log *outlog);

void st_feed_sig_resp_(struct signal_resp_t* lstStructOrderStatusIn, struct symbol_pos_t *lstStructPositionIn, int *sig_cnt, struct signal_t* sigs, struct strat_out_log *outlog);

void st_destroy_();

#endif

