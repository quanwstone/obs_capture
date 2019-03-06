#pragma once
#include"obs.h"

#ifndef DL_D3D11
#define DL_D3D11 "libobs-d3d11.dll"
#endif

struct AddSourceData {
	obs_source_t *source;
	bool visible;
};

class CObsAdapter
{
public:
	CObsAdapter();
	~CObsAdapter();

public:	
	
	bool Obs_startup(const char *local,char *path);

	int  Obs_reset_video(HWND hwnd);

	void Obs_load_all_modules();

	bool Obs_create_source(char *Id);

	bool Obs_create_scene(char *Id);

	bool Obs_add_raw_video_callback();

	bool Obs_start_capture();

	bool Obs_enum_groups_cb(void *data, obs_source_t *source);

	void Obs_picture_start(char *path);

private:
	static void OnRenderWindowCB(void *data, uint32_t cx, uint32_t cy);

	static void OnRawVideoCB(void *param, video_data *pframe);

	void SaveVideoDataToBmp(uint8_t *data, uint32_t linesize);

private:
	
	obs_video_info m_obs_video_info;

	obs_source_t *m_pobs_source_t;

	obs_scene_t *m_pobs_scene_t;

	gs_init_data m_gsInfo;

	unsigned char *m_pchBmp;
	int m_iDH;
	int m_iDW;
};

