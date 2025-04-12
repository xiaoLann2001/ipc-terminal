#include "soapH.h"
#include "soapStub.h"

#include "global.h"
#include "onvif_config.h"

/** Web service operation '__trt__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSources' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__CreateProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse) {
    
    
    return SOAP_OK;
}

/** Web service operation '__trt__GetProfiles' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
    // Profile 个数
    trt__GetProfilesResponse->__sizeProfiles = 2;
    
    // <Profiles>
    trt__GetProfilesResponse->Profiles = (struct tt__Profile *)soap_malloc(soap, sizeof(struct tt__Profile) * trt__GetProfilesResponse->__sizeProfiles);
    memset(trt__GetProfilesResponse->Profiles, '\0', sizeof(struct tt__Profile) * trt__GetProfilesResponse->__sizeProfiles);

    // Profile_1
    int i = 0;
    // <Profiles>/<token>
    trt__GetProfilesResponse->Profiles[i].token = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].token, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].token, rk_param_get_string("video.0:profile_token", "Profile_1"));

    // <Profiles>/<fixed>
    trt__GetProfilesResponse->Profiles[i].fixed = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
    *(trt__GetProfilesResponse->Profiles[i].fixed) = xsd__boolean__true_;
    
    // <Profiles>/<Name>
    trt__GetProfilesResponse->Profiles[i].Name = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].Name, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].Name, rk_param_get_string("video.0:profile_type", "mainStream"));
    
    // <Profiles>/<VideoSourceConfiguration>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoSourceConfiguration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
    // <Profiles>/<VideoSourceConfiguration>/<token>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token, "VideoSourceToken");
    // <Profiles>/<VideoSourceConfiguration>/<Name>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name, "VideoSourceConfig");
    // <Profiles>/<VideoSourceConfiguration>/<UseCount>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->UseCount = 1;
    // <Profiles>/<VideoSourceConfiguration>/<SourceToken>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken, "VideoSource_1");
    // <Profiles>/<VideoSourceConfiguration>/<Bounds>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_malloc(soap, sizeof(struct tt__IntRectangle));
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds, 0, sizeof(struct tt__IntRectangle));
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->x = 0;
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->y = 0;
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->width = rk_param_get_int("video.0:width", 2304);
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->height = rk_param_get_int("video.0:height", 1296);

    // <Profiles>/<VideoEncoderConfiguration>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration)) ;
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration, '\0', sizeof(struct tt__VideoEncoderConfiguration));
    // <Profiles>/<VideoEncoderConfiguration>/<token>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token = (char *)soap_malloc(soap, sizeof(char)*32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token, '\0', sizeof(char)*32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token, "VideoEncoderToken_1");
    // <Profiles>/<VideoEncoderConfiguration>/<Name>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name = (char *)soap_malloc(soap, sizeof(char)*32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name, '\0', sizeof(char)*32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name, "VideoEncoder_1");
    // <Profiles>/<VideoEncoderConfiguration>/<UseCount>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->UseCount = 1;
    // <Profiles>/<VideoEncoderConfiguration>/<Encoding>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    // <Profiles>/<VideoEncoderConfiguration>/<Resolution>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution, '\0', sizeof(struct tt__VideoResolution));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Width = rk_param_get_int("video.0:width", 2304);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Height = rk_param_get_int("video.0:height", 1296);
    // <Profiles>/<VideoEncoderConfiguration>/<Quality>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Quality = 1;
    // <Profiles>/<VideoEncoderConfiguration>/<RateControl>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl, '\0', sizeof(struct tt__VideoRateControl));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->FrameRateLimit = rk_param_get_int("video.0:dst_frame_rate_num", 25);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->BitrateLimit = rk_param_get_int("video.0:max_rate", 2048);
    // <Profiles>/<VideoEncoderConfiguration>/<MPEG4>
    // <Profiles>/<VideoEncoderConfiguration>/<H264>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264, '\0', sizeof(struct tt__H264Configuration));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->GovLength = rk_param_get_int("video.0:gop", 50);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High;
    // <Profiles>/<VideoEncoderConfiguration>/<Multicast>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast, '\0', sizeof(struct tt__MulticastConfiguration));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address, 0, sizeof(struct tt__IPAddress));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address, 0, sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address, "224.1.0.0");
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Port = 40000;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->TTL = 64;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__true_;
    

    // Profile_2
    i = 1;
    // <Profiles>/<token>
    trt__GetProfilesResponse->Profiles[i].token = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].token, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].token, rk_param_get_string("video.1:profile_token", "Profile_2"));

    // <Profiles>/<fixed>
    trt__GetProfilesResponse->Profiles[i].fixed = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
    *(trt__GetProfilesResponse->Profiles[i].fixed) = xsd__boolean__true_;
    
    // <Profiles>/<Name>
    trt__GetProfilesResponse->Profiles[i].Name = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].Name, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].Name, rk_param_get_string("video.1:profile_type", "subStream"));
    
    // <Profiles>/<VideoSourceConfiguration>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoSourceConfiguration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration, 0, sizeof(struct tt__VideoSourceConfiguration));
    // <Profiles>/<VideoSourceConfiguration>/<token>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token, "VideoSourceToken");
    // <Profiles>/<VideoSourceConfiguration>/<Name>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name, "VideoSourceConfig");
    // <Profiles>/<VideoSourceConfiguration>/<UseCount>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->UseCount = 1;
    // <Profiles>/<VideoSourceConfiguration>/<SourceToken>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken = (char *)soap_malloc(soap,sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken, '\0', sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken, "VideoSource_1");
    // <Profiles>/<VideoSourceConfiguration>/<Bounds>
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_malloc(soap, sizeof(struct tt__IntRectangle));
    memset(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds, 0, sizeof(struct tt__IntRectangle));
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->x = 0;
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->y = 0;
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->width = rk_param_get_int("video.1:width", 704);
    trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->height = rk_param_get_int("video.1:height", 576);

    // <Profiles>/<VideoEncoderConfiguration>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration)) ;
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration, '\0', sizeof(struct tt__VideoEncoderConfiguration));
    // <Profiles>/<VideoEncoderConfiguration>/<token>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token = (char *)soap_malloc(soap, sizeof(char)*32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token, '\0', sizeof(char)*32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token, "VideoEncoderToken_2");
    // <Profiles>/<VideoEncoderConfiguration>/<Name>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name = (char *)soap_malloc(soap, sizeof(char)*32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name, '\0', sizeof(char)*32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name, "VideoEncoder_2");
    // <Profiles>/<VideoEncoderConfiguration>/<UseCount>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->UseCount = 1;
    // <Profiles>/<VideoEncoderConfiguration>/<Encoding>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    // <Profiles>/<VideoEncoderConfiguration>/<Resolution>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution, '\0', sizeof(struct tt__VideoResolution));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Width = rk_param_get_int("video.1:width", 704);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Height = rk_param_get_int("video.1:height", 576);
    // <Profiles>/<VideoEncoderConfiguration>/<Quality>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Quality = 1;
    // <Profiles>/<VideoEncoderConfiguration>/<RateControl>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl, '\0', sizeof(struct tt__VideoRateControl));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->FrameRateLimit = rk_param_get_int("video.1:dst_frame_rate_num", 30);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->BitrateLimit = rk_param_get_int("video.1:max_rate", 512);
    // <Profiles>/<VideoEncoderConfiguration>/<MPEG4>
    // <Profiles>/<VideoEncoderConfiguration>/<H264>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264, '\0', sizeof(struct tt__H264Configuration));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->GovLength = rk_param_get_int("video.1:gop", 50);
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High;
    // <Profiles>/<VideoEncoderConfiguration>/<Multicast>
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast, '\0', sizeof(struct tt__MulticastConfiguration));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address, 0, sizeof(struct tt__IPAddress));
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char *)soap_malloc(soap, sizeof(char) * 32);
    memset(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address, 0, sizeof(char) * 32);
    strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address, "224.1.0.0");
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Port = 40000;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->TTL = 64;
    trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__true_;
    
    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* soap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* soap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* soap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* soap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddPTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* soap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* soap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* soap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* soap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* soap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* soap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* soap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* soap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* soap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemovePTZConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* soap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* soap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* soap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* soap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__DeleteProfile' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(struct soap* soap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* soap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* soap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(struct soap* soap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(struct soap* soap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(struct soap* soap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(struct soap* soap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(struct soap* soap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* soap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* soap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* soap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* soap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* soap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* soap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleMetadataConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* soap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* soap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* soap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* soap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* soap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* soap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* soap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* soap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* soap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* soap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* soap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* soap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetGuaranteedNumberOfVideoEncoderInstances' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetStreamUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {
    // 初始化 MediaUri
    trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri *)soap_malloc(soap, sizeof(struct tt__MediaUri));
    memset(trt__GetStreamUriResponse->MediaUri, 0, sizeof(struct tt__MediaUri));

    // 获取设备的ip地址
    char ipaddr[IP_ADDR_LENGTH] = { 0 };
    if (get_local_ip("eth0", ipaddr) < 0) {
		LOG_ERROR("Get local IP address failed\n");
		return SOAP_ERR;
	}

    // 根据各个设备的rtsp协议的uri不同填写对应的值
    trt__GetStreamUriResponse->MediaUri->Uri = (char *)soap_malloc(soap, sizeof(char) * 100);
    memset(trt__GetStreamUriResponse->MediaUri->Uri, '\0', sizeof(char) * 100);

    // 获取 ProfileToken
    char *profile_token = trt__GetStreamUri->ProfileToken;

    // 根据 ProfileToken 设置不同的 URI
    if (strcmp(profile_token, "Profile_1") == 0) {
        sprintf(trt__GetStreamUriResponse->MediaUri->Uri, "rtsp://%s/%s", ipaddr, MAIN_STREAM_PATH);
    } else if (strcmp(profile_token, "Profile_2") == 0) {
        sprintf(trt__GetStreamUriResponse->MediaUri->Uri, "rtsp://%s/%s", ipaddr, SUB_STREAM_PATH);
    } else {
        // 未知的 ProfileToken
        LOG_ERROR("Unknown ProfileToken: %s\n", profile_token);
        return SOAP_ERR;
    }

    // 设置其他参数
    trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__true_;
    trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot  = xsd__boolean__true_;
    trt__GetStreamUriResponse->MediaUri->Timeout = NULL;
    
    return SOAP_OK;
}

/** Web service operation '__trt__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* soap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* soap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* soap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetSnapshotUri' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceModes(struct soap* soap, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceMode(struct soap* soap, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetOSDs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDs(struct soap* soap, struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSD(struct soap* soap, struct _trt__GetOSD *trt__GetOSD, struct _trt__GetOSDResponse *trt__GetOSDResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__GetOSDOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDOptions(struct soap* soap, struct _trt__GetOSDOptions *trt__GetOSDOptions, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__SetOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetOSD(struct soap* soap, struct _trt__SetOSD *trt__SetOSD, struct _trt__SetOSDResponse *trt__SetOSDResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__CreateOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateOSD(struct soap* soap, struct _trt__CreateOSD *trt__CreateOSD, struct _trt__CreateOSDResponse *trt__CreateOSDResponse) {
    return SOAP_OK;
}

/** Web service operation '__trt__DeleteOSD' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteOSD(struct soap* soap, struct _trt__DeleteOSD *trt__DeleteOSD, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse) {
    return SOAP_OK;
}
