#ifndef _SOMEIP_TYPES_H_
#define _SOMEIP_TYPES_H_

typedef struct {
	const uint16 Id;
	const uint16 InstanceId;
	const uint16 MethodId;
	const uint8 MajorVersion;
	const uint32 MinorVersion;
	const uint16 EventId;
} Someip_ServiceType;


typedef struct {
	const uint32 NoOfServices;
	const Someip_ServiceType* Service;

} Someip_InstanceType;


#endif
