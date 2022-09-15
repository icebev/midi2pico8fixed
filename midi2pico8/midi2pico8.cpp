﻿// midi2pico8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <Windows.h>

#include "../RtMidi.h"

std::vector< unsigned char >* old_message;

void mycallback(double deltatime, std::vector< unsigned char > *message, void *userData)
{

	if (message == old_message)
	{
		old_message = NULL;
		return;
	}
	unsigned int nBytes = message->size();
	if (message->at(0) == 144)
	{
		// 36 -> 84
		int note = message->at(1);
		char vks[] = "Q2W3ER5T6Y7UZSXDCVGBHNJM";
		int scs[] =
		{
			0x2c, 0x1f, 0x2d, 0x20, 0x2e, 0x2f, 0x22, 0x30, 0x23, 0x31, 0x24, 0x32,
			0x10, 0x3, 0x11, 0x4, 0x12, 0x13, 0x6, 0x14, 0x7, 0x15, 0x8, 0x16,
		};
		//char keys[] = "zsxdcvgbhnjmq2w3er5t6y7u";
		if (note >= 48 && note < 72)
		{
			//auto key = VkKeyScan(keys[note - 48]);
			auto vk = vks[note - 48];
			auto sc = scs[note - 48];
			auto hCurrentWindow = GetForegroundWindow();

			INPUT ip;
			ip.type = INPUT_KEYBOARD;
			ip.ki.time = 0;
			ip.ki.wVk = vk;
			ip.ki.wScan = sc; // note - 48; //VK_RETURN is the code of Return key

			ip.ki.dwExtraInfo = 0;
			ip.ki.dwFlags = 0;
			SendInput(1, &ip, sizeof(INPUT));
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));

			std::cout << vk;

			old_message = message;
		}
	}
#if 0
	else
	{
		for (unsigned int i = 0; i < nBytes; i++)
			std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
		if (nBytes > 0)
			std::cout << "stamp = " << deltatime << std::endl;
	}
#endif
}

int main()
{
	RtMidiIn *midiin = new RtMidiIn();
	midiin->setCallback(&mycallback);
	midiin->ignoreTypes(true, true, true);

	std::cout << "MIDI to PICO-8\n\n";

	while (true)
	{
		std::cout << "Waiting for a MIDI input device...\n";

		while (midiin->getPortCount() == 0)
			Sleep(200);

		std::cout << "Reading MIDI input from " << midiin->getPortName(0) << "...\n";
		midiin->openPort(0);

		while (midiin->getPortCount() > 0)
			Sleep(200);

		std::cout << "\nDisconnected!\n";
		midiin->closePort();
	}

	delete midiin;
	return 0;
}
