## 标准化协议

### 简介

项目分为两部分：

* 协议层：实现了协议的**帧格式定义**、**帧校验**以及**帧接收**的功能，对外开放几个接口

* 应用层：对**帧**的**数据负载**做处理，其处理方式完全由用户自行定义，可同时实现多个应用，只需初始化对应的实例即可，对外开放了应用对应的实例

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

1. 实例化一个应用：

    * `Recv_Buffxxx`：**应用层**的**环形缓冲区**地址
    * `RECV_BUFF_LENTH_MAX_xxx`：**环形缓冲区**最大长度
    * `Command_Send_xxx`：**应用层**的命令发送回调函数
    * `Command_Recv_xxx`：**应用层**的命令接收回调函数
    * `Report_Send_xxx`：**应用层**的报告发送回调函数
    * `Report_Send_xxx`：**应用层**的报告接收回调函数

   ```c
   PT_APPLICATION_s PT_App_xxx =
   {
       .lock = false,
       .pbuff = Recv_Buffxxx,
       .buff_size = RECV_BUFF_LENTH_MAX_xxx,
       .index_write = 0,
       .index_read = 0,
       .lenth_written = 0,
       .frame_send = {0},
       .frame_recv = {0},
       .pfunc_command_send = Command_Send_xxx,
       .pfunc_command_recv = Command_Recv_xxx,
       .pfunc_report_send = Report_Send_xxx,
       .pfunc_report_recv = Report_Recv_xxx,
   };
   ```

### 使用示例

回调函数模板，以`RS485_Report_Send`为例：

* `RS485.h`：

```c
typedef union
{
	//主动上报 RC_REPORT
	struct
	{
		uint8_t port;
		uint8_t vin[3];
		uint8_t iin[2];
		uint8_t vout[3];
		uint8_t iout[2];
		uint8_t sta;
		uint8_t res[PT_DATA_LENTH_MAX - 12];
	}auto_report_s;


	//查询信息 RC_QUERY_INFO
	struct
	{
		uint8_t ack;
		uint8_t port;
		uint8_t dev_sn_tx[3];
		uint8_t sw_ver_tx[3];
		uint8_t dev_sn_rx[3];
		uint8_t sw_ver_rx[3];
	}query_info_s;


	//触发应答 RC_ON || RC_OFF
	struct
	{
		uint8_t ack;
		uint8_t res[PT_DATA_LENTH_MAX - 1];
	}ack_s;

	//预留
	//...

	//命令码解析错误 RC_CMD_ERR
	struct
	{
		uint8_t err;
		uint8_t rec_cmd;
		uint8_t res[PT_DATA_LENTH_MAX - 2];
	}cmd_recv_err_s;

}PT_PACK_UART_u;
```

* `RS485.h`：

```c
static void RS485_Report_Send(uint8_t cmd, uint8_t rec_cmd)
{
	PT_FRAME_s frame;
	PT_PACK_UART_u pack;
	switch(cmd)
	{
		case RC_CMD_ERR:
		{
			pack.cmd_recv_err_s.err = 0x00;
			pack.cmd_recv_err_s.rec_cmd = rec_cmd;
			memset(pack.cmd_recv_err_s.res, 0, sizeof(pack.cmd_recv_err_s.res));
			break;
		}
		case RC_REPORT:
		{
			pack.auto_report_s.port = RDP_TX;
			pack.auto_report_s.vin[0] = Vin & 0xff;
			pack.auto_report_s.vin[1] = (Vin >> 8) & 0xff;
			pack.auto_report_s.vin[2] = (Vin >> 16) & 0xff;
			pack.auto_report_s.iin[0] = IF & 0xff;
			pack.auto_report_s.iin[1] = (IF >> 8) & 0xff;
			memset(pack.auto_report_s.vout,0,sizeof(pack.auto_report_s.vout));
			memset(pack.auto_report_s.iout,0,sizeof(pack.auto_report_s.iout));
			pack.auto_report_s.sta = PDS_IDLE;
			memset(pack.auto_report_s.res, 0, sizeof(pack.auto_report_s.res));
			break;
		}
		case RC_QUERY_INFO:
		{
			pack.query_info_s.ack = 0;
			pack.query_info_s.port = RDP_TX;
			pack.query_info_s.dev_sn_tx[0] = 'd';
			pack.query_info_s.dev_sn_tx[1] = 'e';
			pack.query_info_s.dev_sn_tx[2] = 'v';
			pack.query_info_s.sw_ver_tx[0] = Convert_str2uint32(SW_VERSION+20, 2);
			pack.query_info_s.sw_ver_tx[1] = Convert_str2uint32(SW_VERSION+23, 2);
			pack.query_info_s.sw_ver_tx[2] = Convert_str2uint32(SUFFIX_NO+1, 2);
			memset(pack.query_info_s.dev_sn_rx, 0, sizeof(pack.query_info_s.dev_sn_rx));
			memset(pack.query_info_s.sw_ver_rx, 0, sizeof(pack.query_info_s.sw_ver_rx));
			break;
		}
		default:
		{
			pack.ack_s.ack = 0;
			memset(pack.ack_s.res, 0, sizeof(pack.ack_s.res));
			break;
		}
	}
	frame.Header = PT_FRAME_HEADER;
	frame.Lenth = PT_FRAME_LENTH;
	frame.Count = count;
	frame.Command = cmd;
	memcpy(frame.Data, &pack, PT_DATA_LENTH_MAX);
	PT_CheckSum(&frame, frame.Checksum);
	count ++;

	trans_buff.dma_len = sizeof(frame);
	memcpy(trans_buff.data, (uint8_t *)&frame, trans_buff.dma_len);
	uart_dma_send((unsigned char*)&trans_buff);
}
```







