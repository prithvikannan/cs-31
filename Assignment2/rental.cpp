#include <iostream>
#include <string>

using namespace std;

int main()
{
	const double BASE_CHARGE_NON_LUXURY = 33;
	const double BASE_CHARGE_LUXURY = 61;
	const double COST_FIRST_100 = 0.27;
	const double COST_NEXT_300_WINTER = 0.27;
	const double COST_NEXT_300_OTHER = 0.21;
	const double COST_ABOVE_400 = 0.19;

	const double FIRST_THRESHOLD = 100;
	const double SECOND_THRESHOLD = 400;

	const int JANUARY = 1;
	const int MARCH = 3;
	const int DECEMBER = 12;

	int startOdometer;
	int endOdometer;
	int rentalDays;
	string customerName;
	string isLuxury;
	int startMonth;

	int milesDriven;
	double total = 0; 

		// Gather the input

	cout << "Odometer at start: ";
	cin >> startOdometer;


	cout << "Odometer at end: ";
	cin >> endOdometer;
	milesDriven = endOdometer - startOdometer; // calculate the difference to find distance driven

	cout << "Rental days: ";
	cin >> rentalDays;

	cout << "Customer name: ";
	cin.ignore(10000, '\n');
	getline(cin, customerName);

	cout << "Luxury car? (y/n): ";
	getline(cin, isLuxury);

	cout << "Month (1=Jan, 2=Feb, etc.): ";
	cin >> startMonth;

	cout << "---" << endl;

		// checking
	if (startOdometer < 0) {
		cout << "The starting odometer reading must be nonnegative." << endl;
		return 1;
	}
	else if (milesDriven < 0) {
		cout << "The final odometer reading must be at least as large as the starting reading." << endl;
		return 1;
	} 
	else if (rentalDays < 0) {
		cout << "The number of rental days must be positive." << endl;
		return 1;
	} 
	else if (customerName == "") {
		cout << "You must enter a customer name." << endl;
		return 1;
	}
	else if (isLuxury != "y" && isLuxury != "n") {
		cout << "If the luxury status is not y or n (must be lower case)." << endl;
		return 1;
	}
	else if (startMonth < JANUARY || startMonth > DECEMBER) {
		cout << "The month number must be in the range 1 through 12." << endl;
		return 1;
	}
		// calculating cost

	/*
	base charge
	*/

	if (isLuxury == "y") // is luxury car
	{
		total += rentalDays * BASE_CHARGE_LUXURY;
	}
	else // is not luxury car
	{
		total += rentalDays * BASE_CHARGE_NON_LUXURY;
	}

	/*
	mileage charge
	*/


	if (milesDriven >= SECOND_THRESHOLD)
	{
		int milesDrivenOver400 = milesDriven - SECOND_THRESHOLD;
		total += milesDrivenOver400 * COST_ABOVE_400;
	}

	if (milesDriven > FIRST_THRESHOLD)
	{

		int milesDrivenFrom100to400 = milesDriven - FIRST_THRESHOLD;

		if (milesDrivenFrom100to400 > (SECOND_THRESHOLD - FIRST_THRESHOLD)) {
			milesDrivenFrom100to400 = (SECOND_THRESHOLD - FIRST_THRESHOLD);
		}

		if (startMonth == DECEMBER || startMonth <= MARCH) // winter month
		{
			total += milesDrivenFrom100to400 * COST_NEXT_300_WINTER;
		}
		else // non-winter month
		{
			total += milesDrivenFrom100to400 * COST_NEXT_300_OTHER;
		}

	}
		
	int milesDrivenUpTo100 = milesDriven;
	if (milesDriven > FIRST_THRESHOLD) {
		milesDrivenUpTo100 = FIRST_THRESHOLD;
	}
	total += milesDrivenUpTo100 * COST_FIRST_100;
		
	cout.setf(ios::fixed);
	cout.precision(2); // 2 decimal points
	cout << "The rental charge for " << customerName << +" is $" << total << endl;
	return 0;

}