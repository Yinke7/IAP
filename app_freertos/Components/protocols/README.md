## 标准化协议

### 简介

项目分为两部分：

* 协议层：位于`driver`文件夹中，实现了协议的**帧格式定义**、**帧校验**以及**帧接收**的功能，对外开放几个接口

* 应用层：位于`app_xxx`文件夹中，对**帧**的**数据负载**做处理，其处理方式完全由用户自行定义，可同时实现多个应用，只需初始化对应的实例即可，对外开放了应用对应的实例

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

### 使用实例

回调函数模板，以`RS485_Report_Send`为例：

* `user_protocol_dbg.h`：

```c
/** 协议命令 **/
typedef enum {
	CB_DBG_REQUEST = 100,
	CB_DBG_EXIT,
	CB_GET_VER,
	CB_GET_PARA_COUNT,
	CB_READALL_PARAS,
	CB_PARA_WRITE,
	CB_PARA_READ,
} CMD_BuiltIn_e;

/** 帧数据Data[PT_DATA_LENTH_MAX]的格式，细分为多种结构 **/
typedef union {
	/** 回复 **/
	struct {
		unsigned char res[PT_DATA_LENTH_MAX];			//空余空间，用于填充内存
	} ver_reply_s;

	/** 请求 **/
	struct {
		unsigned char flag;		//标记
		unsigned char res[PT_DATA_LENTH_MAX - 1];		//空余空间，用于填充内存
	} request_s;

	/** 参数项 **/
	struct {
		unsigned char cmd;		//参数码
		unsigned char value[4];	//参数值
		unsigned char desc[8];	//参数描述
		union {
			struct {
				unsigned char write: 1;
				unsigned char read: 1;
				unsigned char flash: 1;
			} p_s;
			unsigned char properties;
		} prop_s;			//参数属性
	} para_s;

	/** 参数总数目 **/
	struct {
		unsigned char count[4];	//总数目
		unsigned char res[PT_DATA_LENTH_MAX - 4];		//空余空间，用于填充内存
	} get_para_count_s;

	/** 版本信息 **/
	struct {
		unsigned char index;
		unsigned char data[PT_DATA_LENTH_MAX - 1];
	} ver_s;

} PT_PACK_DBG_u;
```

* `user_protocol_dbg.c`：

```c
static void DBG_Command_Recv(void)
{
	unsigned char chksum[2] = {0};
	PT_CheckSum(&PT_App_DBG.frame_recv, chksum);
	if(memcmp(PT_App_DBG.frame_recv.Checksum, chksum, sizeof(PT_App_DBG.frame_recv.Checksum)) == 0)
	{
		switch(PT_App_DBG.frame_recv.Command)
		{
			case CB_DBG_REQUEST:
			{
				
				break;
			}

			case CB_DBG_EXIT:
			{

				break;
			}

			case CB_GET_VER:
			{

				break;
			}

			case CB_GET_PARA_COUNT:
			{

				break;
			}

			case CB_READALL_PARAS:
			{

				break;
			}

			case CB_PARA_WRITE:
			{
                
				break;
			}
			case CB_PARA_READ:
			{
                
				break;
			}

			default:
				break;
		}
	}
}

static void DBG_Report_Send(uint8_t cmd, uint8_t rec_cmd)
{
	PT_FRAME_s frame = {0};
	PT_PACK_DBG_u pack = {0};

	switch(cmd)
	{
		case CB_DBG_REQUEST:
		{
			pack.request_s.flag = buff[0];
			memset(pack.request_s.res, 0, sizeof(pack.request_s.res));
			break;
		}

		case CB_GET_VER:
		{
			memcpy(&pack, buff, lenth);
			break;
		}

		case CB_GET_PARA_COUNT:
		{
			memcpy(pack.get_para_count_s.count, buff, sizeof(pack.get_para_count_s.count));
			memset(pack.get_para_count_s.res, 0, sizeof(pack.get_para_count_s.res));
			break;
		}

		case CB_READALL_PARAS:
		case CB_PARA_READ:
		{
			for(unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s)/ sizeof(DBG_PARA_ITEM_s); idx++)
			{
				DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *)&ParaGroup + idx;
				if(item->cmd == buff[0])
				{
					pack.para_s.cmd = buff[0];
					pack.para_s.prop_s.properties = item->prop_s.properties;
					DBG_U32_To_U8Array(item->value, pack.para_s.value);
					memcpy(pack.para_s.desc, item->desc, sizeof(pack.para_s.desc));
				}
			}
		}

		case CB_PARA_WRITE:
		{
			for(unsigned char idx = 0; idx < sizeof(DBG_PARA_GROUP_s)/ sizeof(DBG_PARA_ITEM_s); idx++)
			{
				DBG_PARA_ITEM_s *item = (DBG_PARA_ITEM_s *)&ParaGroup + idx;
				if(item->cmd == buff[0])
				{
					pack.para_s.cmd = buff[0];
				}
			}
		}

		default:
			break;
	}

	frame.Header = PT_FRAME_HEADER;
	frame.Lenth = PT_FRAME_LENTH;
	frame.Count = count;
	frame.Command = cmd;
	memcpy(frame.Data, &pack, PT_DATA_LENTH_MAX);
	PT_CheckSum(&frame, frame.Checksum);
	count ++;

	//发送
	DBG_Uart_Send_Data((unsigned char *)&frame, sizeof(frame), 20);
}
```







