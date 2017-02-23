
#ifndef __PnDStructOffset_H
#define __PnDStructOffset_H

/***************************PnDStruct中的偏移**********************************/
//;AFN=0CH
//;以PnD->Start为首地址
#define  _CBTime    1
#define  _RC313    (_CBTime +  5 )
#define  _T1F25    (_RC313  +  1 )
#define  _T1F26    (_T1F25  +  46)
#define  _T1F27    (_T1F26  +  52)
#define  _T1F33    (_T1F27  +  23)
#define  _T1F34    (_T1F33  +  85)
#define  _T1F35    (_T1F34  +  85)
#define  _T1F36    (_T1F35  +  70)
#define  _T1F37    (_T1F36  +  70)
#define  _T1F38    (_T1F37  +  85)
#define  _T1F39    (_T1F38  +  85)
#define  _T1F40    (_T1F39  +  70)
#define  _T1F41    (_T1F40  +  70)
#define  _T1F42    (_T1F41  +  20)
#define  _T1F43    (_T1F42  +  20)
#define  _T1F44    (_T1F43  +  20)
#define  _T1F45    (_T1F44  +  20)
#define  _T1F46    (_T1F45  +  20)
#define  _T1F47    (_T1F46  +  20)
#define  _T1F48    (_T1F47  +  20)
//;小时冻
#define  _T1F89    (_T1F48  +  20)
#define  _T1F90    (_T1F89  +  13)
#define  _T1F91    (_T1F90  +  13)
#define  _T1F92    (_T1F91  +  13)
#define  _T1F93    (_T1F92  +  13)
#define  _T1F94    (_T1F93  +  13)
#define  _T1F95    (_T1F94  +  13)
#define  _T1F96    (_T1F95  +  13)
#define  _T1F97    (_T1F96  +  13)
#define  _T1F98    (_T1F97  +  9 )
#define  _T1F99    (_T1F98  +  9 )
#define  _T1F100   (_T1F99  +  9 )
#define  _T1F101   (_T1F100 +  9 )
#define  _T1F102   (_T1F101 +  9 )
#define  _T1F103   (_T1F102 +  9 )
#define  _T1F105   (_T1F103 +  9 )
#define  _T1F106   (_T1F105 +  17)
#define  _T1F107   (_T1F106 +  17)
#define  _T1F108   (_T1F107 +  17)
#define  _T1F109   (_T1F108 +  17)
#define  _T1F110   (_T1F109 +  17)
#define  _T1F111   (_T1F110 +  17)
#define  _T1F112   (_T1F111 +  17)
#define  _T1F113   (_T1F112 +  17)
#define  _T1F114   (_T1F113 +  9 )
#define  _T1F115   (_T1F114 +  9 )
#define  _T1F116   (_T1F115 +  9 )
//;当前小时冻结数据偏移
#define  _T1NF89    (_T1F116 +  9   )
#define  _T1NF90    (_T1NF89  +  13 )
#define  _T1NF91    (_T1NF90  +  13 )
#define  _T1NF92    (_T1NF91  +  13 )
#define  _T1NF93    (_T1NF92  +  13 )
#define  _T1NF94    (_T1NF93  +  13 )
#define  _T1NF95    (_T1NF94  +  13 )
#define  _T1NF96    (_T1NF95  +  13 )
#define  _T1NF97    (_T1NF96  +  13 )
#define  _T1NF98    (_T1NF97  +  9  )
#define  _T1NF99    (_T1NF98  +  9  )
#define  _T1NF100   (_T1NF99  +  9  )
#define  _T1NF101   (_T1NF100 +  9  )
#define  _T1NF102   (_T1NF101 +  9  )
#define  _T1NF103   (_T1NF102 +  9  )
#define  _T1NF105   (_T1NF103 +  9  )
#define  _T1NF106   (_T1NF105 +  17 )
#define  _T1NF107   (_T1NF106 +  17 )
#define  _T1NF108   (_T1NF107 +  17 )
#define  _T1NF109   (_T1NF108 +  17 )
#define  _T1NF110   (_T1NF109 +  17 )
#define  _T1NF111   (_T1NF110 +  17 )
#define  _T1NF112   (_T1NF111 +  17 )
#define  _T1NF113   (_T1NF112 +  17 )
#define  _T1NF114   (_T1NF113 +  9  )
#define  _T1NF115   (_T1NF114 +  9  )
#define  _T1NF116   (_T1NF115 +  9  )


/*******************struct PoPStr 中的偏移***************************/
#define  _PF25     0
#define  _PF26     (_PF25   + 8 )
#define  _PF27     (_PF26   + 28)
#define  _PF28     (_PF27   + 81)
#define  _PF29     (_PF28   + 4 )
#define  _PF30     (_PF29   + 4 )
#define  _PF31     (_PF30   + 4)
#define  _PF32     (_PF31   + 20)

#endif /*__PnDStructOffset_H */
