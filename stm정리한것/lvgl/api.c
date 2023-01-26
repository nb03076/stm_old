	lv_disp_t : 디스플레이 구조체
	lv_disp_get_default()
	
	터치스크린같은 indev는 터치 스크린 read할때
	data->state = LV_INDEV_STATE_PRESSED 처럼
	state를 줘야함
	
	//RTOS 같은 환경
	static mutex_t lvgl_mutex;
	void lvgl_thread(void)
	{	
		while(1) {
		mutex_lock(&lvgl_mutex);
		lv_task_handler();
		mutex_unlock(&lvgl_mutex);
		thread_sleep(10); /* sleep for 10 ms */
		}
	}
	void other_thread(void)
	{
		/* You must always hold the mutex while using LVGL APIs */
		mutex_lock(&lvgl_mutex);
		lv_obj_t *img = lv_img_create(lv_scr_act());
		mutex_unlock(&lvgl_mutex);
		while(1) {
		mutex_lock(&lvgl_mutex);
		/* change to the next image */
		lv_img_set_src(img, next_image);
		mutex_unlock(&lvgl_mutex);
		thread_sleep(2000);
		}
	}
	
	//rotation
	lv_disp_set_rotation(disp, rot)
	
	//obj set xxx
	lv_obj_t* cur_obj = lv_scr_act();
    lv_obj_t* obj = lv_btn_create(cur_obj);

    lv_obj_set_pos(obj, lv_pct(50), lv_pct(50)); //버튼 위치
    lv_obj_set_size(obj, 50, 50); // 버튼 사이즈
	
	
	lv_obj_set_x(obj,-30); //-30같은 음수도 가능!
	lv_obj_set_width(obj,30);
	
	lv_obj_remove_style_all(obj1); /*It removes the set sizes*/
	
	//slider
	lv_slider_set_range(slider1, 0, 100);
	lv_slider_set_value(slider1, 40, LV_ANIM_ON);
	
	
	//object delete
	void lv_obj_del(lv_obj_t * obj);
	lv_obj_del_delayed(obj, 1000);
	
	//screen
	lv_obj_t * scr1 = lv_obj_create(NULL)
	
	//비례값
	lv_pct(value)
	
	//align
	lv_obj_align(obj, align, x, y);
	LV_ALIGN_XXX_XXX
	
	lv_obj_align_to(obj_to_align, reference_obj, align, x, y);
	
	//style
	여러개의 obj를 쉽게 특정 size를 이용해서 처리하기 위해 사용
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_width(&style, 100);
	lv_obj_t * btn = lv_btn_create(lv_scr_act());
	lv_obj_add_style(btn, &style, LV_PART_MAIN);
	
	//translate
	lv_style_set_translate_y(&style_pressed, -100);
	
	//padding
	pad로 시작하는 api들...
	
	//scroll
	lv_obj_add_style(obj, &style_red, LV_STATE_SCROLLED | LV_PART_SCROLLBAR);
	
	//color
	lv_color_t c = lv_color_make(red, green, blue);
	
	//font
	LV_FONT_MONTSERRAT_12 12 px font
	
	
===================================================================
스타일 : border / background / 텍스트 / pad / outline / transform / 
radius / img / shadow / arc(말발굽 모양 slide) / line(라인 여러개 잇는것도 있다)
/transition(lv_style_prop_t 이용)

뭔가 눈에 보기 깔끔한 수치가 
border 기준으로 lv_palette_darken(LV_PALETTE_색,3)
set_border_width 3