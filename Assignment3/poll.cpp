#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>


using namespace std;

bool hasProperSyntax(string pollData);
bool isValidStateForecast(string stateForecast);
bool isValidPartyResult(string partyResult);
bool isValidStateCode(string stateCode);
int tallySeats(string pollData, char party, int& seatTally);
int getNumFromString(string text);

int main()
{
	assert(hasProperSyntax("CT5D,NY9R17D1I,VT,ne3r00D"));
	assert(!hasProperSyntax("ZT5D,NY9R17D1I,VT,ne3r00D"));
	int seats;
	seats = -999;    // so we can detect whether tallySeats sets seats
	assert(tallySeats("CT5D,NY9R17D1I,VT,ne3r00D", 'd', seats) == 0 && seats == 22);
	seats = -999;    // so we can detect whether tallySeats sets seats
	assert(tallySeats("CT5D,NY9R17D1I,VT,ne3r00D", '%', seats) == 2 && seats == -999);
		cout << "All tests succeeded" << endl;
}

bool hasProperSyntax(string pollData) 
{

	string s = pollData;
	string delimiter = ",";

	size_t pos = 0;
	string token;

	// loop iterates over pollData to pull out individual state forecasts, removes state forecast after

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		// cerr << token << endl;
		if (!isValidStateForecast(token)) {
			return false;
		}
		
		if (pos == s.size() - 1) {
			return false;
		}
		s.erase(0, pos + delimiter.length());
	}

	// checks the remaining part of pollData 
	if (s.size() != 0 && !isValidStateForecast(s)) {
		return false;
	}
	return true;
	
}

bool isValidStateForecast(string stateForecast) 
{
	for (int i = 0; i != stateForecast.size(); i++) 
	{
		// can't have any spaces
		if (stateForecast[i] == ' ') 
		{
			return false;
		}
		// can't have any special characters
		if (!isalpha(stateForecast[i]) && !isdigit(stateForecast[i])) {
			return false;
		}
	}

	if (stateForecast.size() >= 2) {
		string stateCode = stateForecast.substr(0, 2);
		
		// check for valid state code
		if (!isValidStateCode(stateCode)) 
		{
			return false;
		}
		int index = 2;

		// loop over party results
		while (index < stateForecast.size())  
		{
			string partyResults = stateForecast.substr(index, stateForecast.size() - index);

			int indexOfNextLetter = 0;

			// search for the next letter
			while (!isalpha(partyResults[indexOfNextLetter])) { 
				indexOfNextLetter++;
				if (indexOfNextLetter > partyResults.size()) {
					return false;
				}
			}

			string partyResult = stateForecast.substr(index, indexOfNextLetter + 1);
			if (!isValidPartyResult(partyResult))
			{
				return false;
			}
			index += partyResult.size();
		}
	}
	else {
		return false;
	}
	return true;
}

bool isValidPartyResult(string partyResult)
{
	// makes sure last character is letter
	if (!isalpha(partyResult[partyResult.size() - 1])) { 
		return false;
	}

	// must be 1 or 2 digits preceding letter
	if (partyResult.size() > 3 || partyResult.size() <= 1) { 
		return false;
	}

	return true;
}
bool isValidStateCode(string stateCode)
{
	for (int i = 0; i < stateCode.size(); i++) {
		stateCode[i] = toupper(stateCode[i]);
	}
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

int getNumFromString(string text) {
	int num = 0;
	for (int i = 0; i != text.size(); i++) {
		int digit = text[text.size() - 1 - i] - '0';
		num += (digit) * pow(10, i);
	}
	return num;
}


int tallySeats(string pollData, char party, int& seatTally) 
{
	// if pollData invalid
	if (!hasProperSyntax(pollData)) { 
		return 1;
	} 

	// if party invalid
	if (!isalpha(party)) { 
		return 2;
	}
	seatTally = 0; // create a counter to hold votes for this party
	string s = pollData;
	string delimiter = ",";
	size_t index = 0;
	string stateForecast;

	// loops over pollData to find state forecasts
	while ((index = s.find(delimiter)) != string::npos) {  
		stateForecast = s.substr(0, index);
	
		string stateCode = stateForecast.substr(0, 2);

		if (!isValidStateCode(stateCode))
		{
			return false;
		}
		int index = 2;

		// loop over party results
		while (index < stateForecast.size()) 
		{
			string partyResults = stateForecast.substr(index, stateForecast.size() - index);
			int indexOfNextLetter = 0;
			while (!isalpha(partyResults[indexOfNextLetter])) {
				indexOfNextLetter++;
			}
			string partyResult = stateForecast.substr(index, indexOfNextLetter + 1);
			if (toupper(partyResult[partyResult.size() - 1]) == toupper(party)) { // if this party result matches our party
				string value = partyResult.substr(0, partyResult.size() - 1);
				seatTally += getNumFromString(value); // add votes to counter
			}
			index += partyResult.size();
		}
		s.erase(0, index + delimiter.length());
	}
	
	// check for remaining state forecast
	if (isValidStateForecast(s)) {
		index = 2;
		while (index < s.size())
		{
			string partyResults = s.substr(index, s.size() - index);
			int indexOfNextLetter = 0;
			while (!isalpha(partyResults[indexOfNextLetter])) {
				indexOfNextLetter++;
			}
			string partyResult = s.substr(index, indexOfNextLetter + 1);
			if (toupper(partyResult[partyResult.size() - 1]) == toupper(party)) {
				string value = partyResult.substr(0, partyResult.size() - 1);
				seatTally += getNumFromString(value);;
			}
			index += partyResult.size();
		}
	}
	return 0;
}