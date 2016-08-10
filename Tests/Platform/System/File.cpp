#include "TestHeader.hpp"

using namespace CYB::Platform::System;

SCENARIO("Touching the unimplemented functions for code coverage1", "[Platform][System][File][Unit]") {
	File(Path(Path::SystemPath::TEMPORARY), File::Mode::READ_WRITE, File::Method::ANY);
}
SCENARIO("Touching the unimplemented functions for code coverage2", "[Platform][System][File][Unit]") {
	const Path Test(Path::SystemPath::TEMPORARY);
	File(Test, File::Mode::READ_WRITE, File::Method::ANY);
}
SCENARIO("Touching the unimplemented functions for code coverage3", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	File(std::move(*reinterpret_cast<File*>(&asdf)));
}
SCENARIO("Touching the unimplemented functions for code coverage4", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.Size();
}
SCENARIO("Touching the unimplemented functions for code coverage5", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.CursorPosition();
}
SCENARIO("Touching the unimplemented functions for code coverage6", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.Seek(0, File::SeekLocation::CURSOR);
}
SCENARIO("Touching the unimplemented functions for code coverage7", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.Read(nullptr, 0);
}
SCENARIO("Touching the unimplemented functions for code coverage8", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.Write(nullptr, 0);
}
SCENARIO("Touching the unimplemented functions for code coverage9", "[Platform][System][File][Unit]") {
	byte asdf[sizeof(File)];
	auto& TheFile(*reinterpret_cast<File*>(&asdf));
	TheFile.Path();
}