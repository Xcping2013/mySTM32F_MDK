电机：顺时针 正转 R；逆时针反转	L	


IO扩展板丝印 ：  输入正序IN13---INPUT13    输出反序OUT9---OUTPUT14

1.双启按键必须为2个按键一起同时按下才动作，当双启1按下后超过30ms再按下双启按键2后为无效动作
2.双启按键触发后的2个气缸动作必须完成后才会进行测试动作
3.修复lidopen双启动作后的采集命令
4.增加LCR coil项目切换命令EPP-IN3WHIS18R-001	
5.不同项目独立对应.C文件与其他框架分开
																--20180228
1.增加四个GPIO用于与GND的切换								    --20180314

1.四个GPIO改为两个LED2和IN3
2.项目切换命令更改为:	type[button online]    type[button offline]    type[lidopen]    type[iac lcr]    type[jabil lcr]
																--20180323	
																
待优化：接收命令固定为小写，发送命令与返回命令为小写;命令格式优化成与INC-MB1616007-B188&B235一样，不必返回2次发生值
	old:int[1]\r\n		int[1]<1>					new: int[1]<1>;		 read_input(1)<1>;		
	old:out[1][1]\r\n	out[1][1]<ok>				new: out[1][1]<ok>;	 set_output(1,1)<ok>

修改SPI发送和接收函数，保证不贴SPI器件时，SPI能自动延时退出，PCB需要增加电容，防止热拔插打坏IC，焊接或者接线错误IC故障
															 ------20180329
															 
IN OUT LED引脚改变	;	SPI1_CS IIC 引脚改变	

EEPROM	AT24C128--->256

LCR Contro

