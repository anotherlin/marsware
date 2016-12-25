#include "sms.h"

uint8_t ReadPort (SMS *sms, int port)
{
	switch (port >>6) {

	case 0:


	case 1:

		return port & 1
			? ReadVCounter(&sms->vdp)
			: ReadHCounter(&sms->vdp);

	case 2: 

		return port & 1 
			? ReadVDPStatus(&sms->vdp)
			: ReadVDPData(&sms->vdp);

	case 3: return 0;
	
	}
}

void WritePort (SMS *sms, int port, int x)
{
	switch (port >> 6) {

	case 0: {

	

		break;

	}



	case 1:  break; /*PSG*/

	case 2: {

		if (port & 1)

			WriteVDPCommand(&sms->vdp, x);

		else

			WriteVDPData(&sms->vdp, x);

		break;

	}

	case 3:
	default:

		break;

	}
}
