# obs_capture
利用obs实现屏幕捕捉


首先需要下载obs源码进行编译,生成obs.lib文件。

运行环境：
需要和obs源码里目录结构一致，如果需要调用plugin和data.

第一阶段实现：

实现monitor-capture功能.
流程：

(1)Obs_startup
用于底层创建obs对象.

(2)Obs_load_all_modules
加载所需要的effect文件和plugin等，这一步最关键的是exe程序和文件的路径结构.

(3)Obs_reset_video
初始化video采集，创建线程用于采集(obs_graphocs_thread).
tick_sources为获取数据，output_frame用于读取数据进行编码，render_dispaly用于数据渲染
tick_sources：通过source内部的video_tick进行数据读取，首先是sources获取数据，其次是scene获取sources上的数据.
都是通过D3D实现,基于gpu内部操作.

output_frame:读取采集的数据放入队列并通过信号量通知vide_thread线程进行数据读取，通过添加的video_callback
进行数据回调.

(4)Obs_create_source
创建sources，obs的插件都是封装为obs_source_info类型,通过id进行对象查找并且调用.


(5)Obs_create_scene
创建scene，数据的渲染流程 sources获取数据-》scene渲染.需要手动进行关联(obs_scene_add)


(6)Obs_add_raw_video_callback
为了获取sources读取的数据，需要手动添加回调函数，最终在video_thread线程进行回调.该video参数可以设置数据格式和类型.
内部通过gpu进行转换.


(7)Obs_start_capture
设置obs_display_add_draw_callback.用于进行窗口绘制.并调用obs_set_output_source进行绑定通道绘制启动.

通过Display的回调进行绘制显示(obs_render_main_texture),为了实现画面的缩放需要通过gs_ortho进行.然后
调用gs_set_viewport进行设置.


第二阶段实现：
实现window -capture功能.
流程：

(1)obs初始化和创建source以及sence的创建和连接都没有区别，唯一的区别是source需要时window-capture插件.
(2)通过obs_source_properties来获取window-caputre内部实现的窗口句柄列表和一些信息，
该步骤主要会带有四种类型的信息，window，cursor，priority，compatibility，
window：对应窗口的句柄等信息.
cursor：对应是否鼠标显示
priority：优先级选择
compatibility：兼容性.
目前只是保存了window的信息，
(3)把保存的window信息，通过obs_data_set_string进行设置，用于修改source对应的setting内容
(4)obs_source_update，内部通过修改临时变量用于更新 window-capture内部的window对象.

问题难点：对于properties内容的理解.






















