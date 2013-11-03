#	Makefile for scripts used by CTN software

SCRIPTS = add-accession-numbers create_common create_icons \
create_icons.94 createdirectory fillImageDBScript his_layout \
icon_script  img_layout \
pacs_layout print_layout start_clients start_his_client start_his_server \
start_img_client start_img_displays start_img_server start_pacs_client \
start_pacs_server start_print_client stop_img_displays

include $(DICOM_MAKE)

application:	$(SCRIPTS)

install:
	chmod 775 $(SCRIPTS)
	cp add-accession-numbers $(DICOM_BIN)
	cp create_common $(DICOM_BIN)
	cp create_icons $(DICOM_BIN)
	cp create_icons.94 $(DICOM_BIN)
	cp createdirectory $(DICOM_BIN)
	cp fillImageDBScript $(DICOM_BIN)
	cp his_layout $(DICOM_BIN)
	cp icon_script $(DICOM_BIN)
	cp img_layout $(DICOM_BIN)
	cp pacs_layout $(DICOM_BIN)
	cp print_layout $(DICOM_BIN)
	cp start_clients $(DICOM_BIN)
	cp start_his_client $(DICOM_BIN)
	cp start_his_server $(DICOM_BIN)
	cp start_img_client $(DICOM_BIN)
	cp start_img_displays $(DICOM_BIN)
	cp start_img_server $(DICOM_BIN)
	cp start_pacs_client $(DICOM_BIN)
	cp start_pacs_server $(DICOM_BIN)
	cp start_print_client $(DICOM_BIN)
	cp stop_img_displays $(DICOM_BIN)

install-clean:	install
	make clean

clean:

indent:

checkin:
	$(CI) add-accession-numbers
	$(CI) checkEnvironment
	$(CI) create_common
	$(CI) create_icons
	$(CI) create_icons.94
	$(CI) fillImageDBScript
	$(CI) his_layout
	$(CI) icon_script
	$(CI) img_layout
	$(CI) pacs_layout
	$(CI) print_layout
	$(CI) start_clients
	$(CI) start_his_client
	$(CI) start_his_server
	$(CI) start_img_client
	$(CI) start_img_displays
	$(CI) start_img_server
	$(CI) start_pacs_client
	$(CI) start_pacs_server
	$(CI) start_print_client
	$(CI) stop_img_displays

checkout:
	$(CO) add-accession-numbers
	$(CO) checkEnvironment
	$(CO) create_common
	$(CO) create_icons
	$(CO) create_icons.94
	$(CO) fillImageDBScript
	$(CO) his_layout
	$(CO) icon_script
	$(CO) img_layout
	$(CO) pacs_layout
	$(CO) print_layout
	$(CO) start_clients
	$(CO) start_his_client
	$(CO) start_his_server
	$(CO) start_img_client
	$(CO) start_img_displays
	$(CO) start_img_server
	$(CO) start_pacs_client
	$(CO) start_pacs_server
	$(CO) start_print_client
	$(CO) stop_img_displays

lint:
