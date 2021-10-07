#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

namespace JFIF
{

enum class SegmentType : unsigned char
{
	TEM = 0x01,		/// (for temporary private used in arithmetic coding)
					// 0x02..0xBF are reserved
	SOF0 = 0xC0,	/// Start of frame (Huffman, non-differential, baseline DCT)
	SOF1 = 0xC1,	/// Start of frame (Huffman, non-differential, extended sequential DCT)
	SOF2 = 0xC2,	/// Start of frame (Huffman, non-differential, progressive DCT)
	SOF3 = 0xC3,	/// Start of frame (Huffman, non-differential, lossless)
	DHT = 0xC4,		/// Define Huffman table(s)
	SOF5 = 0xC5,	/// Start of frame (Huffman, differential, sequential DCT)
	SOF6 = 0xC6,	/// Start of frame (Huffman, differential, progressive DCT)
	SOF7 = 0xC7,	/// Start of frame (Huffman, differential, lossless)
	JPG = 0xC8,		/// Start of frame (arithmetic, non-differential, reserved)
	SOF9 = 0xC9,	/// Start of frame (arithmetic, non-differential, extended sequential DCT)
	SOF10 = 0xCA,	/// Start of frame (arithmetic, non-differential, progressive DCT)
	SOF11 = 0xCB,	/// Start of frame (arithmetic, non-differential, lossless)
	DAC = 0xCC,		/// Define arithmetic coding conditioning(s)
	SOF13 = 0xCD,	/// Start of frame (arithmetic, differential, sequential DCT)
	SOF14 = 0xCE,	/// Start of frame (arithmetic, differential, progressive DCT)
	SOF15 = 0xCF,	/// Start of frame (arithmetic, differential, lossless)
	RST0 = 0xD0,	/// Restart interval termination (mod 8 count 0)
	RST1 = 0xD1,	/// Restart interval termination (mod 8 count 1)
	RST2 = 0xD2,	/// Restart interval termination (mod 8 count 2)
	RST3 = 0xD3,	/// Restart interval termination (mod 8 count 3)
	RST4 = 0xD4,	/// Restart interval termination (mod 8 count 4)
	RST5 = 0xD5,	/// Restart interval termination (mod 8 count 5)
	RST6 = 0xD6,	/// Restart interval termination (mod 8 count 6)
	RST7 = 0xD7,	/// Restart interval termination (mod 8 count 7)
	SOI = 0xD8,		/// Start of image
	EOI = 0xD9,		/// End of image
	SOS = 0xDA,		/// Start of scan
	DQT = 0xDB,		/// Define quantization table(s)
	DNL = 0xDC,		/// Define number of lines
	DRI = 0xDD,		/// Define restart interval
	DHP = 0xDE,		/// Define hierarchical progression
	EXP = 0xDF,		/// Expand reference component(s)
	APP0 = 0xE0,	/// Application segment 0
	APP1 = 0xE1,	/// Application segment 1
	APP2 = 0xE2,	/// Application segment 2
	APP3 = 0xE3,	/// Application segment 3
	APP4 = 0xE4,	/// Application segment 4
	APP5 = 0xE5,	/// Application segment 5
	APP6 = 0xE6,	/// Application segment 6
	APP7 = 0xE7,	/// Application segment 7
	APP8 = 0xE8,	/// Application segment 8
	APP9 = 0xE9,	/// Application segment 9
	APP10 = 0xEA,	/// Application segment 10
	APP11 = 0xEB,	/// Application segment 11
	APP12 = 0xEC,	/// Application segment 12
	APP13 = 0xED,	/// Application segment 13
	APP14 = 0xEE,	/// Application segment 14
	APP15 = 0xEF,	/// Application segment 15
	JPG0 = 0xF0,	/// JPEG extension 0
	JPG1 = 0xF1,	/// JPEG extension 1
	JPG2 = 0xF2,	/// JPEG extension 2
	JPG3 = 0xF3,	/// JPEG extension 3
	JPG4 = 0xF4,	/// JPEG extension 4
	JPG5 = 0xF5,	/// JPEG extension 5
	JPG6 = 0xF6,	/// JPEG extension 6
	SOF48 = 0xF7,	/// JPEG extension 7
	LSE = 0xF8,		/// JPEG extension 8
	JPG9 = 0xF9,	/// JPEG extension 9
	JPG10 = 0xFA,	/// JPEG extension 10
	JPG11 = 0xFB,	/// JPEG extension 11
	JPG12 = 0xFC,	/// JPEG extension 12
	JPG13 = 0xFD,	/// JPEG extension 13
	COM = 0xFE
};


struct Segment
{
	SegmentType type;
	std::streamoff offset;
};

struct PayloadSegment : public Segment
{
};

struct App0Segment : public PayloadSegment
{
	std::array<char, 5> identifier;
};

struct JfifSegment : public App0Segment
{
	std::uint16_t version;
	enum class Units : std::uint8_t	{ none = 0, dpi = 1, dpcm = 2 } units;
	std::uint16_t xDensity, yDensity;
	std::uint8_t thumbnailWidth, thumbnailHeight;
	std::unique_ptr<std::uint8_t> thumbnail;
};

struct JfxxSegment : public App0Segment
{
	std::uint8_t extensionCode;
	// TODO: implement rest
};

auto readSegment(std::istream& stream) -> std::unique_ptr<Segment>;
//auto readApp0Segment(std::istream& stream) -> std::unique_ptr<App0Segment>;
//auto readJfifSegment(std::istream& stream) -> std::unique_ptr<JfifSegment>;
//auto readJfxxSegment(std::istream& stream) -> std::unique_ptr<JfxxSegment>;


}
