/******************************************************************************
    版权所有：
    版 本 号: 1.0
    文 件 名: dlt_104_soe.h
    生成日期: 
    作    者：李科
    功能说明：实现《配电自动化系统应用DL/T634.5104-2009实施细则》
    其他说明：
    修改记录：
*******************************************************************************/
#ifndef _DLT_104_SOE_
#define _DLT_104_SOE_


extern int soe_YX_process(unsigned char port);
extern int soe_YC_process(unsigned char port);
extern int soe_process_init(unsigned char port);


#endif /* _DLT_104_SOE_ */
