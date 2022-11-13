#include "Base64.h"
#include "Base64.helpers.h"

#include <cassert>
#include <cfloat>
#include <chrono>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

void testBase64(const std::string& src)
{
	std::cout << "Source = \"" << src << '"' << std::endl;

	auto encoded = Base64::encode(src);

	std::cout << "Encode = \"" << encoded << '"' << std::endl;

	try
	{
		auto decodedVector = Base64::decode(encoded);
		auto decoded = std::string((const char*)decodedVector.data(), decodedVector.size());
		
		bool passed = (src == decoded);

		if (passed)
		{
			std::cout << "PASSED" << std::endl
				<< std::endl;
		}
		else
		{
			std::cout << "Decode = \"" << decoded << '"' << std::endl;
			std::cout << "FAILED" << std::endl
				<< std::endl;
		}
		
		assert(passed);
	}
	catch(const std::exception& e)
	{
		std::cout << "Error = " << e.what() << std::endl
			<< "FAILED" << std::endl;
		
		assert(false);
	}
}

void testBase64DecodeNoPadding(const std::string& src, const std::string& dstExpected)
{
	std::cout << "Source = " << src << std::endl;

	std::vector<uint8_t> dstVector(Base64::computeDecodedSize(src.size(), src.data()));
	uint64_t dstSize = dstVector.size();

	Base64::Result result = Base64::decode(src.data(), src.size(), dstVector.data(), dstSize);

	if (Base64::succeed(result))
	{
		std::string dst((const char*)dstVector.data(), dstVector.size());
		std::cout << "Decode = " << dst << std::endl;

		if (dst == dstExpected)
		{
			if (result == Base64::Result::MissingPadding)
			{
				std::cout << "PASSED" << std::endl
					<< std::endl;
			}
			else
			{
				std::cout << "Expected warning not received as result ('" << std::to_string((uint32_t)result) << "' instead of '" << std::to_string((uint32_t)result) << "')"
					<< "FAILED" << std::endl
					<< std::endl;
				assert(false);
			}
		}
		else
		{
			std::cout << "Expected = " << dstExpected << std::endl
				<< "FAILED" << std::endl
				<< std::endl;
			assert(false);
		}
	}
	else
	{
		std::cout << "Error = " << std::to_string((uint32_t)result) << std::endl
			<< "FAILED" << std::endl
			<< std::endl;
		assert(false);
	}
}

int main()
{
	// CHECK

	testBase64("");
	testBase64("A");
	testBase64("AB");
	testBase64("ABC");
	testBase64("ABCD");
	testBase64("ABCDE");
	testBase64("ABCDEF");
	testBase64("ABCDEFG");
	testBase64("ABCDEFGH");
	testBase64("ABCDEFGHI");
	testBase64("ABCDEFGHIJ");
	testBase64("ABCDEFGHIJK");
	testBase64("ABCDEFGHIJKL");
	testBase64("ABCDEFGHIJKLM");
	testBase64("ABCDEFGHIJKLMN");
	testBase64("ABCDEFGHIJKLMNO");
	testBase64("ABCDEFGHIJKLMNOP");
	testBase64("ABCDEFGHIJKLMNOPQ");
	testBase64("ABCDEFGHIJKLMNOPQR");
	testBase64("ABCDEFGHIJKLMNOPQRS");
	testBase64("ABCDEFGHIJKLMNOPQRST");
	testBase64("ABCDEFGHIJKLMNOPQRSTU");
	testBase64("ABCDEFGHIJKLMNOPQRSTUV");
	testBase64("ABCDEFGHIJKLMNOPQRSTUVW");
	testBase64("ABCDEFGHIJKLMNOPQRSTUVWX");

	testBase64DecodeNoPadding("QQ", "A");
	testBase64DecodeNoPadding("QQ=", "A");
	testBase64DecodeNoPadding("QUI", "AB");

	std::string source = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque fermentum. Maecenas adipiscing ante non diam sodales hendrerit. Ut velit mauris, egestas sed, gravida nec, ornare ut, mi.Aenean ut orci vel massa suscipit pulvinar.Nulla sollicitudin.Fusce varius, ligula non tempus aliquam, nunc turpis ullamcorper nibh, in tempus sapien eros vitae ligula.Pellentesque rhoncus nunc et augue.Integer id felis.Curabitur aliquet pellentesque diam.Integer quis metus vitae elit lobortis egestas.Lorem ipsum dolor sit amet, consectetuer adipiscing elit.Morbi vel erat non mauris convallis vehicula.Nulla et sapien.Integer tortor tellus, aliquam faucibus, convallis id, congue eu, quam.Mauris ullamcorper felis vitae erat.Proin feugiat, augue non elementum posuere, metus purus iaculis lectus, et tristique ligula justo vitae magna. Aliquam convallis sollicitudin purus.Praesent aliquam, enim at fermentum mollis, ligula massa adipiscing nisl, ac euismod nibh nisl eu lectus.Fusce vulputate sem at sapien.Vivamus leo.Aliquam euismod libero eu enim.Nulla nec felis sed leo placerat imperdiet.Aenean suscipit nulla in justo.Suspendisse cursus rutrum augue.Nulla tincidunt tincidunt mi.Curabitur iaculis, lorem vel rhoncus faucibus, felis magna fermentum augue, et ultricies lacus lorem varius purus.Curabitur eu amet.";
	testBase64(source);

	// BENCHMARK

	int nbTests = 1000;
	double min;
	std::string encoded = Base64::encode(source);
	std::vector<uint8_t> dummyBuffer(encoded.size(), '\0');
	auto dummyBufferPtr = dummyBuffer.data();
	auto dummyBufferSize = dummyBuffer.size();
	std::cout << "Benchmark (best of " << nbTests << " runs)" << std::endl;
	
	// Benchmark encode

	{
		min = DBL_MAX;
		auto srcPtr = source.data();
		auto srcSize = source.size();
		for (int i = 0; i < nbTests; i++)
		{
			auto t1 = std::chrono::high_resolution_clock::now();
			Base64::encode(srcPtr, srcSize, dummyBufferPtr, dummyBufferSize);
			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration<double, std::micro>(t2 - t1).count();
			if (duration < min) [[unlikely]]
			{
				min = duration;
			}
		}
		std::cout << "\tEncode time = " << (min) << " us" << std::endl;
	}

	// Benchmark decode

	{
		min = DBL_MAX;
		auto srcPtr = encoded.data();
		auto srcSize = encoded.size();
		for (int i = 0; i < nbTests; i++)
		{
			auto t1 = std::chrono::high_resolution_clock::now();
			Base64::decode(srcPtr, srcSize, dummyBufferPtr, dummyBufferSize);
			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration<double, std::micro>(t2 - t1).count();
			if (duration < min) [[unlikely]]
			{
				min = duration;
			}
		}
		std::cout << "\tDecode time = " << (min) << " us" << std::endl;
	}

	return 0;
}

#pragma GCC diagnostic pop
