#include "Someip_Types.h"
#include "Someip_Cfg.h"


static Someip_ServiceType Someip_Services[] = {
	{
		.Id = 0x1111,
		.InstanceId = 0x2222,
		.MethodId = 0x3333,
		.MajorVersion = 0,
		.MinorVersion = 0,
		.EventId = 0x8778,
	},
};

const Someip_InstanceType Someip_Instance = {
	.NoOfServices = 1,
	.Service = Someip_Services,
};

