#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <time.h>


struct PackageInfo {
	std::string sType;
	std::string sData;
};

class PackageSD {
public:
	std::string sDate = "";
	std::string sTime = "";
	std::string sLat1 = "";
	std::string sLat2 = "";
	std::string sLon1 = "";
	std::string sLon2 = "";
	std::string sSpeed = "";
	std::string sCourse = "";
	std::string sHeight = "";
	std::string sSats = "";

	void showPackage() {
		std::cout << "Type: SD" << std::endl;
		std::cout << "iYear: " << iYear << std::endl;
		std::cout << "iMonth: " << iMonth << std::endl;
		std::cout << "iDay: " << iDay << std::endl;
		std::cout << "iHours: " << iHours << std::endl;
		std::cout << "iMinutes: " << iMinutes << std::endl;
		std::cout << "iSeconds: " << iSeconds << std::endl;
		std::cout << "dLat1: " << dLat1 << std::endl;
		std::cout << "sLat2: " << sLat2 << std::endl;
		std::cout << "dLon1: " << dLon1 << std::endl;
		std::cout << "sLon2: " << sLon2 << std::endl;
		std::cout << "iSpeed: " << iSpeed << std::endl;
		std::cout << "iCourse: " << iCourse << std::endl;
		std::cout << "iHeight: " << iHeight << std::endl;
		std::cout << "iSats: " << iSats << std::endl << std::endl;
	}

	void parseFromStrings() {
		iYear = std::stoi(sDate.substr(4, 4));
		iMonth = std::stoi(sDate.substr(2, 2));
		iDay = std::stoi(sDate.substr(0, 2));
		iHours = std::stoi(sTime.substr(0, 2));
		iMinutes = std::stoi(sTime.substr(2, 2));
		iSeconds = std::stoi(sTime.substr(4, 2));
		dLat1 = std::stod(sLat1);
		dLon1 = std::stod(sLon1);
		iSpeed = std::stoi(sSpeed);
		iCourse = std::stoi(sCourse);
		iHeight = std::stoi(sHeight);
		iSats = std::stoi(sSats);
	}


	int iYear;
	int iMonth;
	int iDay;
	int iHours;
	int iMinutes;
	int iSeconds;

	double dLat1;
	double dLon1;
	int iSpeed;
	int iCourse;
	int iHeight;
	int iSats;
};


class PackageM {
public:
	std::string sMessage = "";
	void showPackage() {
		std::cout << "Type: M" << std::endl;
		std::cout << "sMessage: " << sMessage << std::endl << std::endl;
	}
};

class Parser {
public:
	Parser() {

	}
	std::deque<std::string> parseLeftSided(std::string sToParse, std::string sDelimeter) {
		size_t uiDelimeterPos = 0;
		while ((uiDelimeterPos = sToParse.find(sDelimeter)) != std::string::npos) {
			parsed.push_back(sToParse.substr(0, uiDelimeterPos));
			sToParse.erase(0, uiDelimeterPos + sDelimeter.length());
		}
		return parsed;
	}
	std::deque<std::string> parseRightSided(std::string sToParse, std::string sDelimeter) {
		size_t uiDelimeterPos = 0;
		while (true) {
			if ((uiDelimeterPos = sToParse.find(sDelimeter)) != std::string::npos) {
				parsed.push_back(sToParse.substr(0, uiDelimeterPos));
				sToParse.erase(0, uiDelimeterPos + sDelimeter.length());
			}
			else {
				parsed.pop_front();
				parsed.push_back(sToParse);
				return parsed;
			}
		}
	}

	std::deque<std::string> parseDoubleSided(std::string sToParse, std::string sDelimeter) {
		size_t uiDelimeterPos = 0;
		while (true) {
			if ((uiDelimeterPos = sToParse.find(sDelimeter)) != std::string::npos) {
				parsed.push_back(sToParse.substr(0, uiDelimeterPos));
				sToParse.erase(0, uiDelimeterPos + sDelimeter.length());
			}
			else {
				parsed.push_back(sToParse);
				return parsed;
			}
		}
	}

	void showParsed() {
		for (auto part : parsed) {
			std::cout << part << std::endl;
		}
	}

	void clear() { parsed.clear(); }


	std::deque<std::string> parsed;
};

class Packages {
public:
	void parsePackages(std::string rawString) {
		PackageInfo packageInfo;
		auto allPackages = parser.parseLeftSided(rawString, "\r\n");
		for (auto package : allPackages) {
			parser.clear();
			parser.parseRightSided(package, "#");
			packageInfo.sType = parser.parsed[0];
			packageInfo.sData = parser.parsed[1];
			if (packageInfo.sType == "M") {
				parser.clear();
				parser.parseDoubleSided(packageInfo.sData, ";");
				PackageM pM;
				pM.sMessage = parser.parsed[0];
				packagesM.push_back(pM);
			}
			else if (packageInfo.sType == "SD") {
				parser.clear();
				parser.parseDoubleSided(packageInfo.sData, ";");
				PackageSD pSD;
				pSD.sDate = parser.parsed[0];
				pSD.sTime = parser.parsed[1];
				pSD.sLat1 = parser.parsed[2];
				pSD.sLat2 = parser.parsed[3];
				pSD.sLon1 = parser.parsed[4];
				pSD.sLon2 = parser.parsed[5];
				pSD.sSpeed = parser.parsed[6];
				pSD.sCourse = parser.parsed[7];
				pSD.sHeight = parser.parsed[8];
				pSD.sSats = parser.parsed[9];
				pSD.parseFromStrings();
				packagesSD.push_back(pSD);
			}
			else { std::cout << "Unknown package type." << std::endl; }
		}
	}

	void showPackages() {
		std::cout << "M packages: ************************************" << std::endl;
		for (auto pM : packagesM) {
			pM.showPackage();
		}

		std::cout << "SD packages: ************************************" << std::endl;
		for (auto pSD : packagesSD) {
			pSD.showPackage();
		}
	}

private:
	std::vector<PackageM> packagesM;
	std::vector<PackageSD> packagesSD;
	Parser parser;
};




int main() {
	//#Type#Data\r\n
	std::string rawString("#SD#04012011;135515;5544.6025;N;03739.6834;E;35;215;110;7\r\n#M#Some message\r\n#SD#04012011;135515;5544.6025;N;03739.6834;E;35;215;110;7\r\n");
	Packages packages;
	packages.parsePackages(rawString);
	packages.showPackages();
	system("pause");
}