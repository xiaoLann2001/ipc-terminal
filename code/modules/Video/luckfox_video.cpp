
#include "luckfox_video.h"

int rkaiq_init()
{
	// RKAIQ 初始化
	RK_BOOL multi_sensor = RK_FALSE;
	const char *iq_dir = "/etc/iqfiles";
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	SAMPLE_COMM_ISP_Init(0, hdr_mode, multi_sensor, iq_dir);
	SAMPLE_COMM_ISP_Run(0);
	SAMPLE_COMM_ISP_SetFrameRate(0, 60);

	return 0;
}

int rkaiq_deinit()
{
	// RKAIQ 反初始化
	SAMPLE_COMM_ISP_Stop(0);

	return 0;
}

int rkmpi_sys_init()
{
	// RK MPI 系统初始化
	if (RK_MPI_SYS_Init() != RK_SUCCESS)
	{
		printf("RK MPI system initialization failed!\n");
		return -1;
	}
	return 0;
}

int rkmpi_sys_deinit()
{
	// RK MPI 系统反初始化
	if (RK_MPI_SYS_Exit() != RK_SUCCESS)
	{
		printf("RK MPI system deinitialization failed!\n");
		return -1;
	}
	return 0;
}

int create_MB_pool(MB_BLK *src_Blk, MB_POOL *src_Pool, int width, int height)
{
	// 创建内存池
	MB_POOL_CONFIG_S PoolCfg;
	memset(&PoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
	PoolCfg.u64MBSize = width * height * 3;
	PoolCfg.u32MBCnt = 1;
	PoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
	*src_Pool = RK_MPI_MB_CreatePool(&PoolCfg);
	*src_Blk = RK_MPI_MB_GetMB(*src_Pool, width * height * 3, RK_TRUE);

	return 0;
}

int destroy_MB_pool(MB_BLK *src_Blk, MB_POOL *src_Pool)
{
	RK_MPI_MB_ReleaseMB(*src_Blk);
	RK_MPI_MB_DestroyPool(*src_Pool);

	return 0;
}

RK_U64 TEST_COMM_GetNowUs()
{
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

int vi_dev_init()
{
	printf("%s\n", __func__);
	int ret = 0;
	int devId = 0;
	int pipeId = devId;

	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	ret = RK_MPI_VI_GetDevAttr(devId, &stDevAttr);
	if (ret == RK_ERR_VI_NOT_CONFIG)
	{
		// 0-1.config dev
		ret = RK_MPI_VI_SetDevAttr(devId, &stDevAttr);
		if (ret != RK_SUCCESS)
		{
			printf("RK_MPI_VI_SetDevAttr %x\n", ret);
			return -1;
		}
	}
	else
	{
		printf("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(devId);
	if (ret != RK_SUCCESS)
	{
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(devId);
		if (ret != RK_SUCCESS)
		{
			printf("RK_MPI_VI_EnableDev %x\n", ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipeId;
		stBindPipe.PipeId[0] = pipeId;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS)
		{
			printf("RK_MPI_VI_SetDevBindPipe %x\n", ret);
			return -1;
		}
	}
	else
	{
		printf("RK_MPI_VI_EnableDev already\n");
	}

	return 0;
}

int vi_dev_deinit()
{
	RK_MPI_VI_DisableDev(0);

	return 0;
}

int vi_chn_init(int pipeId, int channelId, int width, int height, PIXEL_FORMAT_E enPixelFormat)
{
	int ret;
	int buf_cnt = 2;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType =
		VI_V4L2_MEMORY_TYPE_DMABUF; // VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.stFrameRate.s32SrcFrameRate = -1;
	vi_chn_attr.stFrameRate.s32DstFrameRate = -1;
	vi_chn_attr.enPixelFormat = enPixelFormat;
	// vi_chn_attr.enPixelFormat = RK_FMT_RGB888;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 0;
	ret = RK_MPI_VI_SetChnAttr(pipeId, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipeId, channelId);
	if (ret)
	{
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

int vi_chn_deinit(int pipeId, int channelId)
{
	int ret;
	ret = RK_MPI_VI_DisableChn(pipeId, channelId);
	if (ret)
	{
		printf("ERROR: destroy VI error! ret=%d\n", ret);
		return ret;
	}
	printf("RK_MPI_VI_DisableChn success\n");
	return 0;
}

void *vi_get_frame(int pipeId, int viChannelId, int width, int height, VIDEO_FRAME_INFO_S *stViFrame)
{
	if (RK_MPI_VI_GetChnFrame(pipeId, viChannelId, stViFrame, -1) == RK_SUCCESS)
	{
		return RK_MPI_MB_Handle2VirAddr(stViFrame->stVFrame.pMbBlk);
	}
	return NULL;
}

int vi_release_frame(int pipeId, int viChannelId, VIDEO_FRAME_INFO_S *stViFrame)
{
	if (RK_SUCCESS != RK_MPI_VI_ReleaseChnFrame(pipeId, viChannelId, stViFrame))
	{
		printf("RK_MPI_VI_ReleaseChnFrame error\n");
		return -1;
	}
	return 0;
}

int venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType, PIXEL_FORMAT_E enPixelFormat)
{
	// printf("%s\n",__func__);
	int buf_cnt = 4;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(VENC_CHN_ATTR_S));

	// RTSP H264
	venc_chn_attr.stVencAttr.enType = enType;
	venc_chn_attr.stVencAttr.enPixelFormat = enPixelFormat;
	// venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_RGB888;
	venc_chn_attr.stVencAttr.u32Profile = H264E_PROFILE_MAIN;
	venc_chn_attr.stVencAttr.u32PicWidth = width;
	venc_chn_attr.stVencAttr.u32PicHeight = height;
	venc_chn_attr.stVencAttr.u32VirWidth = width;
	venc_chn_attr.stVencAttr.u32VirHeight = height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = buf_cnt;
	venc_chn_attr.stVencAttr.u32BufSize = width * height * 3 / 2;
	venc_chn_attr.stVencAttr.enMirror = MIRROR_NONE;

	venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = 3 * 1024;
	venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 10;
	RK_MPI_VENC_CreateChn(chnId, &venc_chn_attr);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

int venc_deinit(int chnId)
{
	int ret;
	ret = RK_MPI_VENC_StopRecvFrame(chnId);
	ret |= RK_MPI_VENC_DestroyChn(chnId);
	if (ret)
	{
		printf("ERROR: destroy VENC error! ret=%d\n", ret);
		return ret;
	}
	printf("RK_MPI_VENC_DestroyChn success\n");
	return 0;
}

int venc_encode_frame(int vencChannelId, VIDEO_FRAME_INFO_S *venc_frame)
{
	// get vi frame
	static RK_U32 H264_TimeRef = 0;
	venc_frame->stVFrame.u32TimeRef = H264_TimeRef++;
	venc_frame->stVFrame.u64PTS = TEST_COMM_GetNowUs();
	RK_MPI_VENC_SendFrame(vencChannelId, venc_frame, -1);
	return 0;
}

int venc_release_frame(int vencChannelId, VENC_STREAM_S *stFrame)
{
	if (RK_SUCCESS != RK_MPI_VENC_ReleaseStream(vencChannelId, stFrame))
	{
		printf("RK_MPI_VENC_ReleaseStream error\n");
		return -1;
	}
	return 0;
}

int bind_vi_to_venc(int pipeId, MPP_CHN_S *vi_chn, MPP_CHN_S *venc_chn)
{
	// bind
	int ret;
	vi_chn->enModId = RK_ID_VI;
	vi_chn->s32DevId = 0;
	vi_chn->s32ChnId = pipeId;
	venc_chn->enModId = RK_ID_VENC;
	venc_chn->s32DevId = 0;
	venc_chn->s32ChnId = pipeId;
	ret = RK_MPI_SYS_Bind(vi_chn, venc_chn);
	if (ret)
		printf("Bind VI and VENC error! ret=%#x\n", ret);
	else
		printf("Bind VI and VENC success\n");
	return 0;
}

int unbind_vi_to_venc(int pipeId, MPP_CHN_S *vi_chn, MPP_CHN_S *venc_chn)
{
	int ret;
	// unbind
	vi_chn->enModId = RK_ID_VI;
	vi_chn->s32DevId = 0;
	vi_chn->s32ChnId = pipeId;
	venc_chn->enModId = RK_ID_VENC;
	venc_chn->s32DevId = 0;
	venc_chn->s32ChnId = pipeId;
	ret = RK_MPI_SYS_UnBind(vi_chn, venc_chn);
	if (ret)
		printf("Unbind VI and VENC error! ret=%#x\n", ret);
	else
		printf("Unbind VI and VENC success\n");
	return 0;
}

int vpss_init(int VpssChn, int width, int height)
{
	printf("%s\n", __func__);
	int s32Ret;
	VPSS_CHN_ATTR_S stVpssChnAttr;
	VPSS_GRP_ATTR_S stGrpVpssAttr;

	int s32Grp = 0;

	stGrpVpssAttr.u32MaxW = 4096;
	stGrpVpssAttr.u32MaxH = 4096;
	stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
	stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
	stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE;

	stVpssChnAttr.enChnMode = VPSS_CHN_MODE_USER;
	stVpssChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr.enPixelFormat = RK_FMT_RGB888;
	stVpssChnAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssChnAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssChnAttr.u32Width = width;
	stVpssChnAttr.u32Height = height;
	stVpssChnAttr.enCompressMode = COMPRESS_MODE_NONE;

	s32Ret = RK_MPI_VPSS_CreateGrp(s32Grp, &stGrpVpssAttr);
	if (s32Ret != RK_SUCCESS)
	{
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_SetChnAttr(s32Grp, VpssChn, &stVpssChnAttr);
	if (s32Ret != RK_SUCCESS)
	{
		return s32Ret;
	}
	s32Ret = RK_MPI_VPSS_EnableChn(s32Grp, VpssChn);
	if (s32Ret != RK_SUCCESS)
	{
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_StartGrp(s32Grp);
	if (s32Ret != RK_SUCCESS)
	{
		return s32Ret;
	}
	return s32Ret;
}