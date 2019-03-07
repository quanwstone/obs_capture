#pragma once
#include"obs.h"

#ifndef DL_D3D11
#define DL_D3D11 "libobs-d3d11.dll"
#endif

struct STU_PROPERTIES {
	char **chBuf;
	int	iWindowCount;
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

	bool Obs_stop_capture();

	bool Obs_enum_groups_cb(void *data, obs_source_t *source);

	void Obs_picture_start(char *path);

	bool Obs_get_window_properties(STU_PROPERTIES &out_stuproperties);

	bool Obs_set_cursor_show(bool bSet);

	void Obs_window_capture_defer(const char *in_str);

private:
	static void OnRenderWindowCB(void *data, uint32_t cx, uint32_t cy);

	static void OnRawVideoCB(void *param, video_data *pframe);

	void save_video_data_conv_bmp_internal(uint8_t *data, uint32_t linesize);

	void save_window_properties_internal(obs_property_t *in_property, STU_PROPERTIES &out_stuproperties);

private:

	obs_video_info m_obs_video_info;

	obs_source_t *m_pobs_source_t;

	obs_scene_t *m_pobs_scene_t;
	
	obs_sceneitem_t *m_pobs_scene_item;

	gs_init_data m_gsInfo;

	obs_data_t *m_pobs_data_t;

	unsigned char *m_pchBmp;

	int		m_iWindowCount;

	bool m_bCursorShow;
	int m_iDH;
	int m_iDW;
};

