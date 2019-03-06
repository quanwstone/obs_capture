#include "stdafx.h"
#include "ObsAdapter.h"


CObsAdapter::CObsAdapter()
{
	m_pobs_source_t = nullptr;
	m_pobs_scene_t = nullptr;

	m_pchBmp = new unsigned char[1920*1080*4];
	memset(m_pchBmp,0,1920*1080*4);
}


CObsAdapter::~CObsAdapter()
{
}
void AddSource(void *_data, obs_scene_t *scene)
{
	AddSourceData *data = (AddSourceData *)_data;
	obs_sceneitem_t *sceneitem;

	sceneitem = obs_scene_add(scene, data->source);
	obs_sceneitem_set_visible(sceneitem, data->visible);
}


bool CObsAdapter::Obs_enum_groups_cb(void *data, obs_source_t *source)
{
	CObsAdapter *window = (CObsAdapter *)data;
	
	const char *name = obs_source_get_name(source);
	const char *id = obs_source_get_id(source);

	return true;
}
void CObsAdapter::OnRenderWindowCB(void *data, uint32_t cx, uint32_t cy)
{
	CObsAdapter *pThis = (CObsAdapter *)data;
	obs_video_info ovi;

	obs_get_video_info(&ovi);

	gs_viewport_push();
	gs_projection_push();

	gs_ortho(0.0f, float(ovi.base_width), 0.0f, float(ovi.base_height), -100.0f, 100.0f);
	
	gs_set_viewport(0, 0, pThis->m_iDW, pThis->m_iDH);

	obs_render_main_texture();

	gs_projection_pop();
	gs_viewport_pop();

	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}
void CObsAdapter::OnRawVideoCB(void *param, video_data *pframe)
{
	CObsAdapter *pThis = (CObsAdapter *)param;

	//pThis->SaveVideoDataToBmp(pframe->data[0], pframe->linesize[0]);
}
void CObsAdapter::SaveVideoDataToBmp(uint8_t *data, uint32_t linesize)
{
	unsigned char *pT = m_pchBmp;

	for (int i = 0; i < 1080; i++)
	{
		memcpy(pT, data, linesize);
		pT += linesize;
		data += linesize;
	}

	FILE* lpFilePtr = NULL;
	//char lszBmpFileName[512+128];
	BITMAPFILEHEADER lBmpFileHeader;
	BITMAPINFOHEADER lBmpInfoHeader;	//绘制视频图像用到的位图信息
	int liBmpFileSize = 0;
	int nSizeImage = linesize*1080;

	//得到BMP文件大小
	liBmpFileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nSizeImage;

	//填充BMP文件头
	lBmpFileHeader.bfType = 19778;
	lBmpFileHeader.bfSize = liBmpFileSize;
	lBmpFileHeader.bfReserved1 = 0;
	lBmpFileHeader.bfReserved2 = 0;
	lBmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BMP信息头
	lBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	lBmpInfoHeader.biPlanes = 1;
	lBmpInfoHeader.biCompression = BI_RGB;
	lBmpInfoHeader.biXPelsPerMeter = lBmpInfoHeader.biYPelsPerMeter = 0;
	lBmpInfoHeader.biClrImportant = lBmpInfoHeader.biClrUsed = 0;
	lBmpInfoHeader.biSizeImage = nSizeImage;
	lBmpInfoHeader.biWidth = 1920;
	lBmpInfoHeader.biHeight = -1080;
	lBmpInfoHeader.biBitCount = 32;

	char ch_Name[256] = { 0 };
	sprintf_s(ch_Name, "%s%d%s", "c:\\quanwei\\BMP\\PP\\", 1, ".bmp");
	//创建、写入和关闭BMP文件
	fopen_s(&lpFilePtr, ch_Name, "wb+");
	if (lpFilePtr)
	{

		fwrite(&lBmpFileHeader, sizeof(char), sizeof(BITMAPFILEHEADER), lpFilePtr);
		fwrite(&lBmpInfoHeader, sizeof(char), sizeof(BITMAPINFOHEADER), lpFilePtr);
		fwrite(m_pchBmp, sizeof(char), nSizeImage, lpFilePtr);
		fclose(lpFilePtr);
	}
}
int CObsAdapter::Obs_reset_video(HWND hwnd)
{
	RECT re;
	GetClientRect(hwnd, &re);

	memset((void*)&m_gsInfo, 0, sizeof(gs_init_data));
	m_gsInfo.cx = re.right;
	m_gsInfo.cy = re.bottom;
	m_gsInfo.format = GS_RGBA;
	m_gsInfo.zsformat = GS_ZS_NONE;
	m_gsInfo.window.hwnd = hwnd;

	m_iDH = re.bottom;
	m_iDW = re.right;

	m_obs_video_info.adapter = 0;
	m_obs_video_info.base_width = 1920;
	m_obs_video_info.base_height = 1080;
	m_obs_video_info.fps_num = 30000;
	m_obs_video_info.fps_den = 1001;
	m_obs_video_info.graphics_module = DL_D3D11;
	
	m_obs_video_info.scale_type = OBS_SCALE_BICUBIC;
	m_obs_video_info.gpu_conversion = true;
	m_obs_video_info.output_format = VIDEO_FORMAT_RGBA;
	m_obs_video_info.output_width = 1920;
	m_obs_video_info.output_height = 1080;

	int i = obs_reset_video(&m_obs_video_info);
	
	if(i<0)//effect路径设置有问题.
		throw "obs_reset_video failed.";
	
	return true;
}

bool CObsAdapter::Obs_startup(const char *local, char *path)
{
	return obs_startup(local, path, NULL);
}
void CObsAdapter::Obs_load_all_modules()
{
	obs_load_all_modules();
}
bool CObsAdapter::Obs_create_source(char *Id)
{
	m_pobs_source_t = obs_source_create(Id, "", NULL, nullptr);
	if (!m_pobs_source_t)
	{
		throw "obs_source_create failed.";
	}

	return true;
}
bool CObsAdapter::Obs_create_scene(char *Id)
{
	m_pobs_scene_t = obs_scene_create(Id);
	if (!m_pobs_scene_t)
	{
		throw "obs_scene_create failed.";
	}

	obs_sceneitem_t *item = NULL;
	struct vec2 scale;

	vec2_set(&scale, 1.0f, 1.0f);

	item = obs_scene_add(m_pobs_scene_t,m_pobs_source_t);

	obs_sceneitem_set_scale(item, &scale);

	return true;
}

bool CObsAdapter::Obs_start_capture()
{
	obs_set_output_source(0, obs_scene_get_source(m_pobs_scene_t));

	obs_display_t *display = obs_display_create(&m_gsInfo, 0);

	obs_display_add_draw_callback(display, CObsAdapter::OnRenderWindowCB, this);

	return true;
}

bool CObsAdapter::Obs_add_raw_video_callback()
{
	video_scale_info vsi;
	vsi.colorspace = VIDEO_CS_601;
	vsi.format = VIDEO_FORMAT_BGRA;
	vsi.height = 1080;
	vsi.width = 1920;
	vsi.range = VIDEO_RANGE_FULL;

	obs_add_raw_video_callback(&vsi, CObsAdapter::OnRawVideoCB, this);

	return true;
}
void CObsAdapter::Obs_picture_start(char *path)
{

}