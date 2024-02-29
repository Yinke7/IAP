## 调试模式

### 简介

项目分为两部分：

* 协议层：位于`driver`文件夹中，实现了协议的**帧格式定义**、**帧校验**以及**帧接收**的功能，对外开放几个接口
* 应用层：位于`app_dbg`文件夹中，对**帧**的**数据负载**做处理，其处理方式完全由用户自行定义，可同时实现多个应用，只需初始化对应的实例即可，对外开放了应用对应的实例
* protocols
      │
      ├─app_dbg
      │      user_protocol_dbg.c
      │      user_protocol_dbg.h
      │
      └─driver
              user_protocol.c
              user_protocol.h

### 接口说明

#### 协议层

1. `bool PT_Transport_Recv(PT_APPLICATION_s * papp, uint8_t *buff, uint32_t lenth);`
   * `描述`：将从外设（如串口、蓝牙）中获取的数据流缓存到**环形缓冲区**中，其中可能包含了不止一帧数据
   * `papp`：**应用层**的一个实例
   * `buff`：外设数据流缓冲区的地址
   * `lenth`：外设数据流缓冲区的有效长度
   * `返回值`：若成功写入到`papp`的**环形缓冲区**中返回`true`，否则返回`false`
2. `bool PT_Transport_Alaysis(PT_APPLICATION_s *papp);`
   * `描述`：从**环形缓冲区**中提取一个完整的**帧**，若成功提取会将**环形缓冲区**中的这部分数据删除
   * `papp`：应用层的一个实例
   * `返回值`：若成功提取中返回`true`，否则返回`false`
3. `void PT_CheckSum(PT_FRAME_s *pframe, uint8_t *output);`
   * `描述`：对**帧**进行和校验，计算结果为两个字节
   * `papp`：**应用层**的一个实例
   * `output`：校验和结果存放地址

#### 应用层

* `Recv_Buff_DBG`：**应用层**的**环形缓冲区**地址
* `RECV_BUFF_DBG_LENTH_MAX`：**环形缓冲区**最大长度
* `DBG_Command_Send`：**应用层**的命令发送回调函数
* `DBG_Command_Recv`：**应用层**的命令接收回调函数
* `DBG_Report_Send`：**应用层**的报告发送回调函数
* `DBG_Report_Recv`：**应用层**的报告接收回调函数

### 使用方法

#### 移植

1. 将`protocols`文件夹复制到工程中

2. `protocols/driver`文件夹内容不需修改

3. `protocols/app_dbg/user_protocol_dbg.h`:

   只需在宏`DBG_TAB`中做修改，一步到位。

   * 以`X_MACRO(Vi1_L, 111, 0, "Vi1_L", 1, 1, 1)`为例
     1. `Vi1_L`为变量名，在程序中通过结构体访问成员变量的方式直接使用
     2. `111`为参数的值
     3. `0`为参数命令码，用于对接上位机
     4. `"Vi1_L"`为参数的描述，是一个字符串记得用双引号包起来
     5. 最后三个值为参数的属性，依次对应：是否可写、是否可读、是否可以对Flash访问，用`0/1`做区分

   ```c
   #define DBG_TAB \
   		X_MACRO(Vi1_L, 111, 0, "Vi1_L", 1, 1, 1) 	\
   		X_MACRO(Vi1_H, 222, 1, "Vi1_H", 1, 1, 1) 	\
   		X_MACRO(Vi2_L, 333, 2, "Vi2_L", 1, 1, 1) 	\
   		X_MACRO(Vi2_H, 444, 5, "Vi2_H", 1, 1, 1) 	\
   		X_MACRO(Vi3_L, 555, 7, "Vi3_L", 1, 1, 1) 	\
   		X_MACRO(Vi3_H, 666, 9, "Vi3_H", 1, 1, 1) 	\
   		/*X_MACRO(n, v, c, d, w, r, f)*/
   ```

#### 初始化

1. 初始化版本信息，调用`DBG_Version_Combine`将硬件版本、软件版本、广播名组合起来，在配合使用上位机时会用到，只需上电执行一次

   ```c
   DBG_Version_Combine(HW_VERSION, SW_VERSION, telink_adv_trigger_paring_8258_User);
   ```

2. 初始化参数表

   ```c
   DBG_Init_Params();
   ```

#### 接收串口数据

1. 在串口中断函数中，将数据保存到`PT_App_DBG`的环形缓冲区中

   ```c
   void app_uart_test_irq_proc(void)
   {
       // ...
       	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_RX){
   			dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
   			// ...    
               PT_Transport_Recv(&PT_App_DBG, rec_buff.data, rec_buff.dma_len);
               // ...
           }
       // ...
   }
   ```

#### 解析数据

1. 在`while`循环中，持续解析并处理`PT_App_DBG`的缓冲区中的数据，循环周期最好`>50ms`

   ```c
   	if(Flag_protocol_en == 1)
   	{
   		Flag_protocol_en = 0;
   		if(PT_Transport_Alaysis(&PT_App_DBG))
   		{
   			PT_App_DBG.pfunc_command_recv();
   		}
   	}
   ```

#### 注意事项

1. 解析数据中用到的`Flag_protocol_en`标记需要用户自行定义，并在定时器中置`1`（也可使用用其他方法实现，只要能保证周期执行解析就OK）











