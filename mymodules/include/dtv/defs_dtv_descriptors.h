#ifndef _DEFS_DESCRIPTORS_H_
#define _DEFS_DESCRIPTORS_H_

/**************************************************************************************
 SDT table
 **************************************************************************************/
#define SERVICE_DESCRIPTOR_SERVICE_PROVIDER_NAME_LENGTH 32
#define SERVICE_DESCRIPTOR_SERVICE_NAME_LENGTH 32
struct  ServiceDes{
	RMuint8 service_type;
	RMuint8 service_provider_name_length;
	RMuint8 service_name_length;
	RMuint8 service_provider_name_char[SERVICE_DESCRIPTOR_SERVICE_PROVIDER_NAME_LENGTH + 1];
	RMuint8 service_name_char[SERVICE_DESCRIPTOR_SERVICE_NAME_LENGTH + 1];
};

/**************************************************************************************
 EIT table
 **************************************************************************************/
struct ComponentDes {
	
	RMuint8 stream_content;
	RMuint8 component_type;
	RMuint8 ISO_639_language_code[3];
};

#define SHORT_EVENT_DESCRIPTOR_EVENT_NAME_LENGTH	TITLE_LEN
#define SHORT_EVENT_DESCRIPTOR_TEXT_LENGTH			SHORT_DES_LEN

struct ShortEventDes{
	RMuint8	ISO_639_language_code[3];
	RMuint8 event_name_length;
	RMuint8 event_name_char[SHORT_EVENT_DESCRIPTOR_EVENT_NAME_LENGTH + 1];
	RMuint8 text_length;
	RMuint8	text_char[SHORT_EVENT_DESCRIPTOR_TEXT_LENGTH + 1];
};


#define EXTENDED_EVENT_DESCRIPTOR_ITEM_DESCRIPTION_LENGTH 20
#define EXTENDED_EVENT_DESCRIPTOR_ITEM_LENGTH 20
#define EXTENDED_EVENT_DESCRIPTOR_TEXT_LENGTH EXTEND_DES_LEN
struct ExtendedEventDes{
	RMuint8 descriptor_length;
	RMuint8 descriptor_number:4;
	RMuint8 last_descriptor_number:4;
	RMuint8	ISO_639_language_code[3];
	RMuint8 length_of_items;
	
	RMuint8 item_description_length;
	RMuint8 item_length;
	RMuint8 text_length;
	
	RMuint8 item_description_char[EXTENDED_EVENT_DESCRIPTOR_ITEM_DESCRIPTION_LENGTH + 1];
	RMuint8 item_char[EXTENDED_EVENT_DESCRIPTOR_ITEM_LENGTH + 1];
	RMuint8 text_char[EXTENDED_EVENT_DESCRIPTOR_TEXT_LENGTH + 1];
};

#define  MAX_COUNTRYS 16
struct ParentalRatingDesRecord {
	RMuint8 country_code[3];
	RMuint8 rating;
};

struct ParentalRatingDes {
	RMuint8 num_record;
	struct ParentalRatingDesRecord record[MAX_COUNTRYS];
};


/**************************************************************************************
 PMT table
 **************************************************************************************/
typedef struct PMTParentalRatingDes{
	RMuint8 country_code[3];
	RMuint8 rating;
}SParentalRating;

#define MAX_DATA_SERVICE 16
struct VbiDataDes {
	struct {
		RMuint8 data_service_id;		
	}astDataService[MAX_DATA_SERVICE];
	RMuint8 uiTotalService;	
};

#define MAX_TTX_DES 32
struct TeletextDes
{
	struct {
		RMuint8 ISO_639_language_code[3];
		RMuint8 uiTeletextType;
		RMuint8 uiTeletextMagazineNumber;
		RMuint8 uiTeletextPageNumber;
	}astTtxDes[MAX_TTX_DES];
	RMuint8 uiNumTTXDes;
};

#define MAX_SUBTITLE_DES 64
struct SubtitlingDes {
	struct {
		RMuint8		ISO_639_language_code[3];
		RMuint8		subtitling_type;
		RMuint16	composition_page_id;
		RMuint16	ancillary_page_id;
	}astSubtitleDes[MAX_SUBTITLE_DES];
	RMuint8 uiNumSubtitleDes;
};


/**************************************************************************************
 NIT table
 **************************************************************************************/

#define MAX_SERVICE_NUMBER 50
struct SatelliteDeliverySystemDes {
	RMuint32	frequency;
	RMreal		orbital_position;
	RMuint8		west_east_flag;
	RMuint8		polarization;
	RMuint8		roll_off;
	RMuint8		modulation_system;
	RMuint8		modulation_type;
	RMuint32	symbol_rate;
	RMuint8		FEC_inner;
};

struct CableDeliverySystemDes {
	RMuint32	frequency;
	RMuint8		modulation;
	RMuint32	symbol_rate;
	RMuint8		FEC_inner;
	RMuint16	transport_stream_id;
};

struct ServiceListDes {
	RMuint16	service_id[MAX_SERVICE_NUMBER];
	RMuint8		service_type[MAX_SERVICE_NUMBER];
	RMuint8 	n;
    RMuint8     uiNumServiceList;
};

#define MAX_LOGICAL_CHANNEL_NUMBER 64
struct LogicalChannelDes{
	struct {
		RMuint16 service_id;
		RMuint8	 visible_service_flag;
		RMuint16 logical_channel_number;
	}aLogicalChannelInfo[MAX_LOGICAL_CHANNEL_NUMBER];
	RMuint8 uiNumLogicalChannelInfo;
};

/**************************************************************************************
 TOT table
 **************************************************************************************/
struct LocalTimeOffsetDes{
	RMuint8		country_code[3];
	RMuint8		country_region_id;
	RMuint8		local_time_offset_polariry;
	RMuint16	local_time_offset;
	RMuint8		time_of_change[5];
	RMuint16	next_time_offset;
};

#ifdef DTV_SMK_CAS_SUPPORT
//CA Table
struct CADes {
	RMuint16 ca_system_id;
	RMuint16 ca_pid;
};
#endif //DTV_SMK_CAS_SUPPORT

#endif  // _DEFS_DESCRIPTORS_H_
