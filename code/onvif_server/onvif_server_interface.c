#include "soapH.h"
#include "soapStub.h"

#include "log.h"
#include "utils.h"
#include "onvif_config.h"

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

/** Web service one-way operation 'SOAP_ENV__Fault' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail) {
    return 0;
}

/** Web service one-way operation '__wsdd__Hello' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello) {
    return 0;
}

/** Web service one-way operation '__wsdd__Bye' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye) {
    return 0;
}

/** Web service one-way operation '__wsdd__Probe' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe) {
    // return 0;

	LOG_DEBUG("__wsdd__Probe start !\n");
 
	unsigned char macaddr[MAC_ADDR_LENGTH] = { 0 };
	char ipaddr[IP_ADDR_LENGTH] = { 0 };
	char _XAddr[INFO_LENGTH] = { 0 };
	char _HwId[1024] = { 0 };
 
	wsdd__ProbeMatchesType ProbeMatches;
	ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));

	ProbeMatches.ProbeMatch->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap, sizeof(struct wsdd__ScopesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap, sizeof(struct wsa__ReferencePropertiesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap, sizeof(struct wsa__ReferenceParametersType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap, sizeof(struct wsa__ServiceNameType));

	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *) * SMALL_INFO_LENGTH);
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);

	// 获取 MAC 地址，并生成 HwId
	if (get_local_mac("eth0", macaddr) < 0) {
		LOG_ERROR("Get MAC address failed\n");
		return SOAP_ERR;
	}
	sprintf(_HwId, "urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X",
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

	// 获取 IP 地址，并生成 XAddr
	if (get_local_ip("eth0", ipaddr) < 0) {
		LOG_ERROR("Get local IP address failed\n");
		return SOAP_ERR;
	}
	sprintf(_XAddr, "http://%s/onvif/device_service", ipaddr);
	LOG_DEBUG("_XAddr ==== %s\n", _XAddr);

	ProbeMatches.__sizeProbeMatch = 1;

	ProbeMatches.ProbeMatch->Scopes->__item = (char *)soap_malloc(soap, 1024);
	memset(ProbeMatches.ProbeMatch->Scopes->__item, 0, 1024);

	const char *scopes_message =
		"onvif://www.onvif.org/type/NetworkVideoTransmitter\r\n"
		"onvif://www.onvif.org/Profile/Streaming\r\n"
		"onvif://www.onvif.org/Profile/Q/Operational\r\n"
		"onvif://www.onvif.org/hardware/HD1080P\r\n"
		"onvif://www.onvif.org/name/discover_test\r\n"
		"onvif://www.onvif.org/location/city/ChongQing\r\n"
		"onvif://www.onvif.org/location/country/China\r\n";
	strcat(ProbeMatches.ProbeMatch->Scopes->__item, scopes_message);
	ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;

	// 填充
	ProbeMatches.ProbeMatch->XAddrs = soap_strdup(soap, _XAddr);
	ProbeMatches.ProbeMatch->Types = soap_strdup(soap, wsdd__Probe->Types);
	printf("wsdd__Probe->Types=%s\n", wsdd__Probe->Types);
	ProbeMatches.ProbeMatch->MetadataVersion = 1;

	// 可选项初始化
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;

	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = soap_strdup(soap, "ttl");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = soap_strdup(soap, "Any");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute = soap_strdup(soap, "Attribute");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.Address = soap_strdup(soap, _HwId);

	soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
	soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
	soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));
	soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
	soap->header->wsa__RelatesTo->RelationshipType = NULL;
	soap->header->wsa__RelatesTo->__anyAttribute = NULL;

	soap->header->wsa__MessageID = soap_strdup(soap, _HwId + 4);

	if (SOAP_OK == soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches)) {
		printf("send ProbeMatches success !\n");
		return SOAP_OK;
	}

	printf("[%d] soap error: %d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
	return soap->error;
}

/** Web service one-way operation '__wsdd__ProbeMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches) {
    return 0;
}

/** Web service one-way operation '__wsdd__Resolve' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve) {
    return 0;
}

/** Web service one-way operation '__wsdd__ResolveMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches) {
    return 0;
}

/** Web service operation '__tdn__Hello' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse) {
    return 0;
}

/** Web service operation '__tdn__Bye' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse) {
    return 0;
}

/** Web service operation '__tdn__Probe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse) {
    return 0;
}

