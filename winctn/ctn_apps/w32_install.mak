install:
	cd "archive_agent"
	$(MAKE)/f archive_agent.mak CFG="archive_agent - Win32 Release"
	copy Release\archive_agent.exe $(CTN_TARGET)\bin
	cd ".."

	cd "archive_cleaner"
	$(MAKE)/f archive_cleaner.mak CFG="archive_cleaner - Win32 Release"
	copy Release\archive_cleaner.exe $(CTN_TARGET)\bin
	cd ".."

	cd "archive_server"
	$(MAKE)/f archive_server.mak CFG="archive_server - Win32 Release"
	copy Release\archive_server.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ctn_version"
	$(MAKE)/f ctn_version.mak CFG="ctn_version - Win32 Release"
	copy Release\ctn_version.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_add_fragments"
	$(MAKE)/f dcm_add_fragments.mak CFG="dcm_add_fragments- Win32 Release"
	copy Release\dcm_add_fragments.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_create_object"
	$(MAKE)/f dcm_create_object.mak CFG="dcm_create_object - Win32 Release"
	copy Release\dcm_create_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_ctnto10"
	$(MAKE)/f dcm_ctnto10.mak CFG="dcm_ctnto10 - Win32 Release"
	copy Release\dcm_ctnto10.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_diff"
	$(MAKE)/f dcm_diff.mak CFG="dcm_diff - Win32 Release"
	copy Release\dcm_diff.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_compressed"
	$(MAKE)/f dcm_dump_compressed.mak CFG="dcm_dump_compressed - Win32 Release"
	copy Release\dcm_dump_compressed.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_element"
	$(MAKE)/f dcm_dump_element.mak CFG="dcm_dump_element - Win32 Release"
	copy Release\dcm_dump_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_file"
	$(MAKE)/f dcm_dump_file.mak CFG="dcm_dump_file - Win32 Release"
	copy Release\dcm_dump_file.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_make_object"
	$(MAKE)/f dcm_make_object.mak CFG="dcm_make_object - Win32 Release"
	copy Release\dcm_make_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_map_to_8"
	$(MAKE)/f dcm_map_to_8.mak CFG="dcm_map_to_8 - Win32 Release"
	copy Release\dcm_map_to_8.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_modify_elements"
	$(MAKE)/f dcm_modify_elements.mak CFG="dcm_modify_elements - Win32 Release"
	copy Release\dcm_modify_elements.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_modify_object"
	$(MAKE)/f dcm_modify_object.mak CFG="dcm_modify_object - Win32 Release"
	copy Release\dcm_modify_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_print_dictionary"
	$(MAKE)/f dcm_print_dictionary.mak CFG="dcm_print_dictionary - Win32 Release"
	copy Release\dcm_print_dictionary.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_replace_element"
	$(MAKE)/f dcm_replace_element.mak CFG="dcm_replace_element - Win32 Release"
	copy Release\dcm_replace_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_resize"
	$(MAKE)/f dcm_resize.mak CFG="dcm_resize - Win32 Release"
	copy Release\dcm_resize.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_rm_element"
	$(MAKE)/f dcm_rm_element.mak CFG="dcm_rm_element - Win32 Release"
	copy Release\dcm_rm_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_rm_group"
	$(MAKE)/f dcm_rm_group.mak CFG="dcm_rm_group - Win32 Release"
	copy Release\dcm_rm_group.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_to_html"
	$(MAKE)/f dcm_to_html.mak CFG="dcm_to_html - Win32 Release"
	copy Release\dcm_to_html.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_vr_patterns"
	$(MAKE)/f dcm_vr_patterns.mak CFG="dcm_vr_patterns - Win32 Release"
	copy Release\dcm_vr_patterns.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_w_disp"
	$(MAKE)/f dcm_w_disp.mak CFG="dcm_w_disp - Win32 Release"
	copy Release\dcm_w_disp.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dicom_echo"
	$(MAKE)/f dicom_echo.mak CFG="dicom_echo - Win32 Release"
	copy Release\dicom_echo.exe $(CTN_TARGET)\bin
	cd ".."

	cd "load_control"
	$(MAKE)/f load_control.mak CFG="load_control - Win32 Release"
	copy Release\load_control.exe $(CTN_TARGET)\bin
	cd ".."

	cd "send_image"
	$(MAKE)/f send_image.mak CFG="send_image - Win32 Release"
	copy Release\send_image.exe $(CTN_TARGET)\bin
	cd ".."

	cd "simple_storage"
	$(MAKE)/f simple_storage.mak CFG="simple_storage - Win32 Release"
	copy Release\simple_storage.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttdelete"
	$(MAKE)/f ttdelete.mak CFG="ttdelete - Win32 Release"
	copy Release\ttdelete.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttinsert"
	$(MAKE)/f ttinsert.mak CFG="ttinsert - Win32 Release"
	copy Release\ttinsert.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttlayout"
	$(MAKE)/f ttlayout.mak CFG="ttlayout - Win32 Release"
	copy Release\ttlayout.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttselect"
	$(MAKE)/f ttselect.mak CFG="ttselect - Win32 Release"
	copy Release\ttselect.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttunique"
	$(MAKE)/f ttunique.mak CFG="ttunique - Win32 Release"
	copy Release\ttunique.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttupdate"
	$(MAKE)/f ttupdate.mak CFG="ttupdate - Win32 Release"
	copy Release\ttupdate.exe $(CTN_TARGET)\bin
	cd ".."

clean:
	cd "archive_agent"
	$(MAKE)/f archive_agent.mak CFG="archive_agent - Win32 Release" clean
	cd ".."

	cd "archive_cleaner"
	$(MAKE)/f archive_cleaner.mak CFG="archive_cleaner - Win32 Release" clean
	cd ".."

	cd "archive_server"
	$(MAKE)/f archive_server.mak CFG="archive_server - Win32 Release" clean
	cd ".."

	cd "ctn_version"
	$(MAKE)/f ctn_version.mak CFG="ctn_version - Win32 Release" clean
	cd ".."

	cd "dcm_add_fragments"
	$(MAKE)/f dcm_add_fragments.mak CFG="dcm_add_fragments- Win32 Release" clean
	cd ".."

	cd "dcm_create_object"
	$(MAKE)/f dcm_create_object.mak CFG="dcm_create_object - Win32 Release" clean
	cd ".."

	cd "dcm_ctnto10"
	$(MAKE)/f dcm_ctnto10.mak CFG="dcm_ctnto10 - Win32 Release" clean
	cd ".."

	cd "dcm_diff"
	$(MAKE)/f dcm_diff.mak CFG="dcm_diff - Win32 Release" clean
	cd ".."

	cd "dcm_dump_compressed"
	$(MAKE)/f dcm_dump_compressed.mak CFG="dcm_dump_compressed - Win32 Release" clean
	cd ".."

	cd "dcm_dump_element"
	$(MAKE)/f dcm_dump_element.mak CFG="dcm_dump_element - Win32 Release" clean
	cd ".."

	cd "dcm_dump_file"
	$(MAKE)/f dcm_dump_file.mak CFG="dcm_dump_file - Win32 Release" clean
	cd ".."

	cd "dcm_make_object"
	$(MAKE)/f dcm_make_object.mak CFG="dcm_make_object - Win32 Release" clean
	cd ".."

	cd "dcm_map_to_8"
	$(MAKE)/f dcm_map_to_8.mak CFG="dcm_map_to_8 - Win32 Release" clean
	cd ".."

	cd "dcm_modify_elements"
	$(MAKE)/f dcm_modify_elements.mak CFG="dcm_modify_elements - Win32 Release" clean
	cd ".."

	cd "dcm_modify_object"
	$(MAKE)/f dcm_modify_object.mak CFG="dcm_modify_object - Win32 Release" clean
	cd ".."

	cd "dcm_print_dictionary"
	$(MAKE)/f dcm_print_dictionary.mak CFG="dcm_print_dictionary - Win32 Release" clean
	cd ".."

	cd "dcm_replace_element"
	$(MAKE)/f dcm_replace_element.mak CFG="dcm_replace_element - Win32 Release" clean
	cd ".."

	cd "dcm_resize"
	$(MAKE)/f dcm_resize.mak CFG="dcm_resize - Win32 Release" clean
	cd ".."

	cd "dcm_rm_element"
	$(MAKE)/f dcm_rm_element.mak CFG="dcm_rm_element - Win32 Release" clean
	cd ".."

	cd "dcm_rm_group"
	$(MAKE)/f dcm_rm_group.mak CFG="dcm_rm_group - Win32 Release" clean
	cd ".."

	cd "dcm_to_html"
	$(MAKE)/f dcm_to_html.mak CFG="dcm_to_html - Win32 Release" clean
	cd ".."

	cd "dcm_w_disp"
	$(MAKE)/f dcm_w_disp.mak CFG="dcm_w_disp - Win32 Release" clean
	cd ".."

	cd "dicom_echo"
	$(MAKE)/f dicom_echo.mak CFG="dicom_echo - Win32 Release" clean
	cd ".."

	cd "load_control"
	$(MAKE)/f load_control.mak CFG="load_control - Win32 Release" clean
	cd ".."

	cd "send_image"
	$(MAKE)/f send_image.mak CFG="send_image - Win32 Release" clean
	cd ".."

	cd "simple_storage"
	$(MAKE)/f simple_storage.mak CFG="simple_storage - Win32 Release" clean
	cd ".."

	cd "ttdelete"
	$(MAKE)/f ttdelete.mak CFG="ttdelete - Win32 Release" clean
	cd ".."

	cd "ttinsert"
	$(MAKE)/f ttinsert.mak CFG="ttinsert - Win32 Release" clean
	cd ".."

	cd "ttlayout"
	$(MAKE)/f ttlayout.mak CFG="ttlayout - Win32 Release" clean
	cd ".."

	cd "ttselect"
	$(MAKE)/f ttselect.mak CFG="ttselect - Win32 Release" clean
	cd ".."

	cd "ttunique"
	$(MAKE)/f ttunique.mak CFG="ttunique - Win32 Release" clean
	cd ".."

	cd "ttupdate"
	$(MAKE)/f ttupdate.mak CFG="ttupdate - Win32 Release" clean
	cd ".."

	cd "txt_to_hl7"
	$(MAKE)/f txt_to_hl7.mak CFG="txt_to_hl7 - Win32 Release" clean
	cd ".."

install-debug:
	cd "archive_agent"
	$(MAKE)/f archive_agent.mak CFG="archive_agent - Win32 Debug"
	copy Debug\archive_agent.exe $(CTN_TARGET)\bin
	cd ".."

	cd "archive_cleaner"
	$(MAKE)/f archive_cleaner.mak CFG="archive_cleaner - Win32 Debug"
	copy Debug\archive_cleaner.exe $(CTN_TARGET)\bin
	cd ".."

	cd "archive_server"
	$(MAKE)/f archive_server.mak CFG="archive_server - Win32 Debug"
	copy Debug\archive_server.exe $(CTN_TARGET)\bin
	cd ".."


	cd "ctn_version"
	$(MAKE)/f ctn_version.mak CFG="ctn_version - Win32 Debug"
	copy Debug\ctn_version.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_add_fragments"
	$(MAKE)/f dcm_add_fragments.mak CFG="dcm_add_fragments - Win32 Debug"
	copy Debug\dcm_add_fragments.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_create_object"
	$(MAKE)/f dcm_create_object.mak CFG="dcm_create_object - Win32 Debug"
	copy Debug\dcm_create_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_ctnto10"
	$(MAKE)/f dcm_ctnto10.mak CFG="dcm_ctnto10 - Win32 Debug"
	copy Debug\dcm_ctnto10.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_diff"
	$(MAKE)/f dcm_diff.mak CFG="dcm_diff - Win32 Debug"
	copy Debug\dcm_diff.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_compressed"
	$(MAKE)/f dcm_dump_compressed.mak CFG="dcm_dump_compressed - Win32 Debug"
	copy Debug\dcm_dump_compressed.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_element"
	$(MAKE)/f dcm_dump_element.mak CFG="dcm_dump_element - Win32 Debug"
	copy Debug\dcm_dump_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_dump_file"
	$(MAKE)/f dcm_dump_file.mak CFG="dcm_dump_file - Win32 Debug"
	copy Debug\dcm_dump_file.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_make_object"
	$(MAKE)/f dcm_make_object.mak CFG="dcm_make_object - Win32 Debug"
	copy Debug\dcm_make_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_map_to_8"
	$(MAKE)/f dcm_map_to_8.mak CFG="dcm_map_to_8 - Win32 Debug"
	copy Debug\dcm_map_to_8.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_modify_elements"
	$(MAKE)/f dcm_modify_elements.mak CFG="dcm_modify_elements - Win32 Debug"
	copy Debug\dcm_modify_elements.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_modify_object"
	$(MAKE)/f dcm_modify_object.mak CFG="dcm_modify_object - Win32 Debug"
	copy Debug\dcm_modify_object.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_print_dictionary"
	$(MAKE)/f dcm_print_dictionary.mak CFG="dcm_print_dictionary - Win32 Debug"
	copy Debug\dcm_print_dictionary.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_replace_element"
	$(MAKE)/f dcm_replace_element.mak CFG="dcm_replace_element - Win32 Debug"
	copy Debug\dcm_replace_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_resize"
	$(MAKE)/f dcm_resize.mak CFG="dcm_resize - Win32 Debug"
	copy Debug\dcm_resize.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_rm_element"
	$(MAKE)/f dcm_rm_element.mak CFG="dcm_rm_element - Win32 Debug"
	copy Debug\dcm_rm_element.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_rm_group"
	$(MAKE)/f dcm_rm_group.mak CFG="dcm_rm_group - Win32 Debug"
	copy Debug\dcm_rm_group.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_to_html"
	$(MAKE)/f dcm_to_html.mak CFG="dcm_to_html - Win32 Debug"
	copy Debug\dcm_to_html.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_to_text"
	$(MAKE)/f dcm_to_text.mak CFG="dcm_to_text - Win32 Debug"
	copy Debug\dcm_to_text.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_vr_patterns"
	$(MAKE)/f dcm_vr_patterns.mak CFG="dcm_vr_patterns - Win32 Debug"
	copy Debug\dcm_vr_patterns.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dcm_w_disp"
	$(MAKE)/f dcm_w_disp.mak CFG="dcm_w_disp - Win32 Debug"
	copy Debug\dcm_w_disp.exe $(CTN_TARGET)\bin
	cd ".."

	cd "dicom_echo"
	$(MAKE)/f dicom_echo.mak CFG="dicom_echo - Win32 Debug"
	copy Debug\dicom_echo.exe $(CTN_TARGET)\bin
	cd ".."

	cd "load_control"
	$(MAKE)/f load_control.mak CFG="load_control - Win32 Debug"
	copy Debug\load_control.exe $(CTN_TARGET)\bin
	cd ".."

	cd "send_image"
	$(MAKE)/f send_image.mak CFG="send_image - Win32 Debug"
	copy Debug\send_image.exe $(CTN_TARGET)\bin
	cd ".."

	cd "simple_storage"
	$(MAKE)/f simple_storage.mak CFG="simple_storage - Win32 Debug"
	copy Debug\simple_storage.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttdelete"
	$(MAKE)/f ttdelete.mak CFG="ttdelete - Win32 Debug"
	copy Debug\ttdelete.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttinsert"
	$(MAKE)/f ttinsert.mak CFG="ttinsert - Win32 Debug"
	copy Debug\ttinsert.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttlayout"
	$(MAKE)/f ttlayout.mak CFG="ttlayout - Win32 Debug"
	copy Debug\ttlayout.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttselect"
	$(MAKE)/f ttselect.mak CFG="ttselect - Win32 Debug"
	copy Debug\ttselect.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttunique"
	$(MAKE)/f ttunique.mak CFG="ttunique - Win32 Debug"
	copy Debug\ttunique.exe $(CTN_TARGET)\bin
	cd ".."

	cd "ttupdate"
	$(MAKE)/f ttupdate.mak CFG="ttupdate - Win32 Debug"
	copy Debug\ttupdate.exe $(CTN_TARGET)\bin
	cd ".."


clean-debug:
	cd "archive_agent"
	$(MAKE)/f archive_agent.mak CFG="archive_agent - Win32 Debug" clean
	cd ".."

	cd "archive_cleaner"
	$(MAKE)/f archive_cleaner.mak CFG="archive_cleaner - Win32 Debug" clean
	cd ".."

	cd "archive_server"
	$(MAKE)/f archive_server.mak CFG="archive_server - Win32 Debug" clean
	cd ".."

	cd "ctn_version"
	$(MAKE)/f ctn_version.mak CFG="ctn_version - Win32 Debug" clean
	cd ".."

	cd "dcm_add_fragments"
	$(MAKE)/f dcm_add_fragments.mak CFG="dcm_add_fragments - Win32 Debug" clean
	cd ".."

	cd "dcm_create_object"
	$(MAKE)/f dcm_create_object.mak CFG="dcm_create_object - Win32 Debug" clean
	cd ".."

	cd "dcm_ctnto10"
	$(MAKE)/f dcm_ctnto10.mak CFG="dcm_ctnto10 - Win32 Debug" clean
	cd ".."

	cd "dcm_diff"
	$(MAKE)/f dcm_diff.mak CFG="dcm_diff - Win32 Debug" clean
	cd ".."

	cd "dcm_dump_compressed"
	$(MAKE)/f dcm_dump_compressed.mak CFG="dcm_dump_compressed - Win32 Debug" clean
	cd ".."

	cd "dcm_dump_element"
	$(MAKE)/f dcm_dump_element.mak CFG="dcm_dump_element - Win32 Debug" clean
	cd ".."

	cd "dcm_dump_file"
	$(MAKE)/f dcm_dump_file.mak CFG="dcm_dump_file - Win32 Debug" clean
	cd ".."

	cd "dcm_make_object"
	$(MAKE)/f dcm_make_object.mak CFG="dcm_make_object - Win32 Debug" clean
	cd ".."

	cd "dcm_map_to_8"
	$(MAKE)/f dcm_map_to_8.mak CFG="dcm_map_to_8 - Win32 Debug" clean
	cd ".."

	cd "dcm_modify_elements"
	$(MAKE)/f dcm_modify_elements.mak CFG="dcm_modify_elements - Win32 Debug" clean
	cd ".."

	cd "dcm_modify_object"
	$(MAKE)/f dcm_modify_object.mak CFG="dcm_modify_object - Win32 Debug" clean
	cd ".."

	cd "dcm_print_dictionary"
	$(MAKE)/f dcm_print_dictionary.mak CFG="dcm_print_dictionary - Win32 Debug" clean
	cd ".."

	cd "dcm_replace_element"
	$(MAKE)/f dcm_replace_element.mak CFG="dcm_replace_element - Win32 Debug" clean
	cd ".."

	cd "dcm_resize"
	$(MAKE)/f dcm_resize.mak CFG="dcm_resize - Win32 Debug" clean
	cd ".."

	cd "dcm_rm_element"
	$(MAKE)/f dcm_rm_element.mak CFG="dcm_rm_element - Win32 Debug" clean
	cd ".."

	cd "dcm_rm_group"
	$(MAKE)/f dcm_rm_group.mak CFG="dcm_rm_group - Win32 Debug" clean
	cd ".."

	cd "dcm_to_html"
	$(MAKE)/f dcm_to_html.mak CFG="dcm_to_html - Win32 Debug" clean
	cd ".."

	cd "dcm_vr_patterns"
	$(MAKE)/f dcm_vr_patterns.mak CFG="dcm_vr_patterns - Win32 Debug" clean
	cd ".."

	cd "dcm_w_disp"
	$(MAKE)/f dcm_w_disp.mak CFG="dcm_w_disp - Win32 Debug" clean
	cd ".."

	cd "dicom_echo"
	$(MAKE)/f dicom_echo.mak CFG="dicom_echo - Win32 Debug" clean
	cd ".."

	cd "load_control"
	$(MAKE)/f load_control.mak CFG="load_control - Win32 Debug" clean
	cd ".."

	cd "send_image"
	$(MAKE)/f send_image.mak CFG="send_image - Win32 Debug" clean
	cd ".."

	cd "simple_storage"
	$(MAKE)/f simple_storage.mak CFG="simple_storage - Win32 Debug" clean
	cd ".."

	cd "ttdelete"
	$(MAKE)/f ttdelete.mak CFG="ttdelete - Win32 Debug" clean
	cd ".."

	cd "ttinsert"
	$(MAKE)/f ttinsert.mak CFG="ttinsert - Win32 Debug" clean
	cd ".."

	cd "ttlayout"
	$(MAKE)/f ttlayout.mak CFG="ttlayout - Win32 Debug" clean
	cd ".."

	cd "ttselect"
	$(MAKE)/f ttselect.mak CFG="ttselect - Win32 Debug" clean
	cd ".."

	cd "ttunique"
	$(MAKE)/f ttunique.mak CFG="ttunique - Win32 Debug" clean
	cd ".."

	cd "ttupdate"
	$(MAKE)/f ttupdate.mak CFG="ttupdate - Win32 Debug" clean
	cd ".."

