#include "Miscellaneous.hpp"

#include "../Logger.hpp"
#include "../Emulator.hpp"
#include "../Chipset/Chipset.hpp"
#include "../Chipset/CPU.hpp"

#include <sstream>
#include <iomanip>

namespace casioemu
{
	constexpr uint16_t Miscellaneous::addr [];

	void Miscellaneous::Initialise()
	{
		region_dsr.Setup(0xF000, 1, "Miscellaneous/DSR", this, [](MMURegion *region, size_t) {
			return (uint8_t)((Miscellaneous *)region->userdata)->emulator.chipset.cpu.impl_last_dsr;
		}, [](MMURegion *region, size_t, uint8_t data) {
			Miscellaneous* self = (Miscellaneous *)region->userdata;
			self->emulator.chipset.cpu.impl_last_dsr = data & self->emulator.chipset.cpu.dsr_mask;
		}, emulator);

		if(emulator.hardware_id == HW_CLASSWIZ) {
			//Only tested on fx-991cnx
			region_F004.Setup(0xF004, 1, "Miscellaneous/DataSegAccess", this, [](MMURegion* region, size_t) {
				return (uint8_t)((Miscellaneous*)region->userdata)->emulator.chipset.SegmentAccess;
			}, [](MMURegion* region, size_t, uint8_t data) {
				Miscellaneous* self = (Miscellaneous *)region->userdata;
				self->emulator.chipset.SegmentAccess = data & 1;
			}, emulator);
		}

		// * TODO: figure out what these are

		int n_byte;
		switch (emulator.hardware_id)
		{
		case HW_ES_PLUS:
		case HW_FX_5800P:
			n_byte = 1;
			break;
		case HW_CLASSWIZ:
			n_byte = 5;
			break;
		case HW_CLASSWIZ_II:
			n_byte = 5;
			break;
		default:
			PANIC("Unknown hardware_id\n");
		}
		for (int i = 0; i < n_byte; ++ i)
		{
			std::ostringstream stream;
			stream << "Miscellaneous/Unknown/" << std::hex << std::uppercase << addr[i] << "*1";
			region[i].Setup(addr[i], 1, stream.str(), &data[i], MMURegion::DefaultRead<uint8_t>, MMURegion::DefaultWrite<uint8_t>, emulator);
		}
	}

	void Miscellaneous::Tick() {
		
	}

	void Miscellaneous::Reset() {
		if(emulator.hardware_id == HW_FX_5800P)
			emulator.chipset.InputToPort(0, 3, true);
	}
}
