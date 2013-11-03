

install:
	nmake/f w32_install.mak directories
	nmake/f w32_install.mak headers
	nmake/f w32_install.mak library_r
	nmake/f w32_install.mak apps_r
	nmake/f w32_install.mak runtime_x

clean:
	nmake/f w32_install.mak library_r_clean
	nmake/f w32_install.mak apps_r_clean

javactn:	java_r

directories:
	scripts\createdirectory D:\ctn
	scripts\createdirectory D:\ctn\bin
	scripts\createdirectory D:\ctn\lib
	scripts\createdirectory D:\ctn\runtime
	scripts\createdirectory D:\ctn_install

headers:
	cd winctn\scripts
	headerexport.bat
	cd ..\..

library_r:
	cd winctn\ctn_lib
	nmake/f ctn_lib.mak CFG="ctn_lib - Win32 Release" ALL
	cd ..\..

java_r:
	cd javactn
	nmake/f w32_install.mak install
	cd ..
	cd winctn\javactn
	nmake/f javactn.mak CFG="javactn - Win32 Release" all
	copy Release\javactn.dll D:\ctn\bin
	cd ..\..

apps_r:
	cd winctn\ctn_apps
	nmake/f w32_install.mak install
	cd ..

runtime_x:
	cd runtime
	copy ctn_cfg.txt D:\ctn\runtime
	cd ..

library_r_clean:
	cd winctn\ctn_lib
	nmake/f ctn_lib.mak CFG="ctn_lib - Win32 Release" clean
	cd ..\..

apps_r_clean:
	cd winctn\ctn_apps
	cd archive_agent
	nmake/f archive_agent.mak CFG="archive_agent - Win32 Release" clean
	cd ..
	cd archive_cleaner
	nmake/f archive_cleaner.mak CFG="archive_cleaner - Win32 Release" clean
	cd ..
	cd archive_server
	nmake/f archive_server.mak CFG="archive_server - Win32 Release" clean
	cd ..
	cd clone_study
	nmake/f clone_study.mak CFG="clone_study - Win32 Release" clean
	cd ..
	cd ctn_version
	nmake/f ctn_version.mak CFG="ctn_version - Win32 Release" clean
	cd ..
	cd dcm_create_object
	nmake/f dcm_create_object.mak CFG="dcm_create_object - Win32 Release" clean
	cd ..
	cd dcm_ctnto10
	nmake/f dcm_ctnto10.mak CFG="dcm_ctnto10 - Win32 Release" clean
	cd ..
	cd dcm_diff
	nmake/f dcm_diff.mak CFG="dcm_diff - Win32 Release" clean
	cd ..
	cd dcm_dump_compressed
	nmake/f dcm_dump_compressed.mak CFG="dcm_dump_compressed - Win32 Release" clean
	cd ..
	cd dcm_dump_element
	nmake/f dcm_dump_element.mak CFG="dcm_dump_element - Win32 Release" clean
	cd ..
	cd dcm_dump_file
	nmake/f dcm_dump_file.mak CFG="dcm_dump_file - Win32 Release" clean
	cd ..
	cd dcm_make_object
	nmake/f dcm_make_object.mak CFG="dcm_make_object - Win32 Release" clean
	cd ..
	cd dcm_map_to_8
	nmake/f dcm_map_to_8.mak CFG="dcm_map_to_8 - Win32 Release" clean
	cd ..
	cd dcm_modify_elements
	nmake/f dcm_modify_elements.mak CFG="dcm_modify_elements - Win32 Release" clean
	cd ..
	cd dcm_modify_object
	nmake/f dcm_modify_object.mak CFG="dcm_modify_object - Win32 Release" clean
	cd ..
	cd dcm_print_dictionary
	nmake/f dcm_print_dictionary.mak CFG="dcm_print_dictionary - Win32 Release" clean
	cd ..
	cd dcm_replace_element
	nmake/f dcm_replace_element.mak CFG="dcm_replace_element - Win32 Release" clean
	cd ..
	cd dcm_resize
	nmake/f dcm_resize.mak CFG="dcm_resize - Win32 Release" clean
	cd ..
	cd dcm_rm_element
	nmake/f dcm_rm_element.mak CFG="dcm_rm_element - Win32 Release" clean
	cd ..
	cd dcm_rm_group
	nmake/f dcm_rm_group.mak CFG="dcm_rm_group - Win32 Release" clean
	cd ..
	cd dcm_template
	nmake/f dcm_template.mak CFG="dcm_template - Win32 Release" clean
	cd ..
	cd dcm_to_html
	nmake/f dcm_to_html.mak CFG="dcm_to_html - Win32 Release" clean
	cd ..
	cd dcm_verify
	nmake/f dcm_verify.mak CFG="dcm_verify - Win32 Release" clean
	cd ..
	cd dcm_w_disp
	nmake/f dcm_w_disp.mak CFG="dcm_w_disp - Win32 Release" clean
	cd ..
	cd dicom_echo
	nmake/f dicom_echo.mak CFG="dicom_echo - Win32 Release" clean
	cd ..
	cd dump_commit_requests
	nmake/f dump_commit_requests.mak CFG="dump_commit_requests - Win32 Release" clean
	cd ..
	cd load_control
	nmake/f load_control.mak CFG="load_control - Win32 Release" clean
	cd ..
	cd query_client
	nmake/f query_client.mak CFG="query_client - Win32 Release" clean
	cd ..
	cd send_image
	nmake/f send_image.mak CFG="send_image - Win32 Release" clean
	cd ..
	cd simple_storage
	nmake/f simple_storage.mak CFG="simple_storage - Win32 Release" clean
	cd ..
	cd storage_classes
	nmake/f storage_classes.mak CFG="storage_classes - Win32 Release" clean
	cd ..
	cd ttdelete
	nmake/f ttdelete.mak CFG="ttdelete - Win32 Release" clean
	cd ..
	cd ttinsert
	nmake/f ttinsert.mak CFG="ttinsert - Win32 Release" clean
	cd ..
	cd ttlayout
	nmake/f ttlayout.mak CFG="ttlayout - Win32 Release" clean
	cd ..
	cd ttselect
	nmake/f ttselect.mak CFG="ttselect - Win32 Release" clean
	cd ..
	cd ttunique
	nmake/f ttunique.mak CFG="ttunique - Win32 Release" clean
	cd ..
	cd ttupdate
	nmake/f ttupdate.mak CFG="ttupdate - Win32 Release" clean
	cd ..
	cd ..\..


