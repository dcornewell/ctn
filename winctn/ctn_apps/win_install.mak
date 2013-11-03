# Makefile to install application binaries

debug_install:
	copy archive_server\Debug\archive_server.exe $(DICOM_BIN)
	copy dcm_create_object\Debug\dcm_create_object.exe $(DICOM_BIN)
	copy dcm_ctnto10\Debug\dcm_ctnto10.exe $(DICOM_BIN)
	copy dcm_diff\Debug\dcm_diff.exe $(DICOM_BIN)
	copy dcm_dump_compressed\Debug\dcm_dump_compressed.exe $(DICOM_BIN)
	copy dcm_dump_element\Debug\dcm_dump_element.exe $(DICOM_BIN)
	copy dcm_dump_file\Debug\dcm_dump_file.exe $(DICOM_BIN)
	copy dcm_make_object\Debug\dcm_make_object.exe $(DICOM_BIN)
	copy dcm_map_to_8\Debug\dcm_map_to_8.exe $(DICOM_BIN)
	copy dcm_modify_elements\Debug\dcm_modify_elements.exe $(DICOM_BIN)
	copy dcm_modify_object\Debug\dcm_modify_object.exe $(DICOM_BIN)
	copy dcm_print_dictionary\Debug\dcm_print_dictionary.exe $(DICOM_BIN)
	copy dcm_template\Debug\dcm_template.exe $(DICOM_BIN)
	copy dcm_verify\Debug\dcm_verify.exe $(DICOM_BIN)
	copy dcm_w_disp\Debug\dcm_w_disp.exe $(DICOM_BIN)
	copy dicom_echo\Debug\dicom_echo.exe $(DICOM_BIN)
	copy load_control\Debug\load_control.exe $(DICOM_BIN)
	copy send_image\Debug\send_image.exe $(DICOM_BIN)
	copy simple_storage\Debug\simple_storage.exe $(DICOM_BIN)

debug_install_unix:
	cp archive_server/Debug/archive_server.exe ../ntbinaries
	cp dcm_ctnto10/Debug/dcm_ctnto10.exe ../ntbinaries
	cp dcm_diff/Debug/dcm_diff.exe ../ntbinaries
	cp dcm_dump_element/Debug/dcm_dump_element.exe ../ntbinaries
	cp dcm_dump_file/Debug/dcm_dump_file.exe ../ntbinaries
	cp dcm_map_to_8/Debug/dcm_map_to_8.exe ../ntbinaries
	cp dcm_modify_elements/Debug/dcm_modify_elements.exe ../ntbinaries
	cp dcm_modify_object/Debug/dcm_modify_object.exe ../ntbinaries
	cp dcm_print_dictionary/Debug/dcm_print_dictionary.exe ../ntbinaries
	cp dcm_resize/Debug/dcm_resize.exe ../ntbinaries
	cp dcm_rm_element/Debug/dcm_rm_element.exe ../ntbinaries
	cp dcm_rm_group/Debug/dcm_rm_group.exe ../ntbinaries
	cp dcm_make_object/Debug/dcm_make_object.exe ../ntbinaries
	cp dcm_verify/Debug/dcm_verify.exe ../ntbinaries
	cp dcm_w_disp/Debug/dcm_w_disp.exe ../ntbinaries
	cp dicom_echo/Debug/dicom_echo.exe ../ntbinaries
	cp load_control/Debug/load_control.exe ../ntbinaries
	cp mwlQuery/Debug/mwlquery.exe ../ntbinaries
	cp print_server/Debug/print_server.exe ../ntbinaries
	cp query_client/Debug/query_client.exe ../ntbinaries
	cp send_image/Debug/send_image.exe ../ntbinaries
	cp simple_storage/Debug/simple_storage.exe ../ntbinaries
	cp ttdelete/Debug/ttdelete.exe ../ntbinaries
	cp ttinsert/Debug/ttinsert.exe ../ntbinaries
	cp ttlayout/Debug/ttlayout.exe ../ntbinaries
	cp ttselect/Debug/ttselect.exe ../ntbinaries
	cp ttunique/Debug/ttunique.exe ../ntbinaries
	cp ttupdate/Debug/ttupdate.exe ../ntbinaries
