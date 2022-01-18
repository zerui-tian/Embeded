extern char LCD1602_ReadBusy();
extern void LCD1602_WriteCom(char com);
extern void LCD1602_WriteData(char dat);
extern void LCD1602_Init();
extern void LCD1602_Refresh(char stringOnLine1[], char stringOnLine2[]);