#include<bits/stdc++.h>
using namespace std;

const int earth_radius_miles = 3959;

// Assuming functionalities for inserting and reading records from database is present.

struct property {
	int id; 						// unique id assigned to a property
	double latitude, longitude; 	// in radians
	long long price; 				// in USD
	int bedrooms, bathrooms;		
};

/*
	Returns match value for Distance. Max. match value=30. 
	Match value decreases as distance from desired latitude and longitude increases.
	Returns -1.0 (invalid) if distance becomes greater than 10 miles.
*/
float cal_match_radius(double latitude1, double longitude1, double latitude2, double longitude2) {
	double u, v, w;
	u = sin((latitude2-latitude1)/2);
	v = sin((longitude2-longitude1)/2);
	w = cos(latitude1)*cos(latitude2);
	double distance = 2.0*earth_radius_miles*asin(sqrt(u*u+w*v*v));
	if(distance <= 2.0) return 30.0;
	if(distance > 10.0) return -1.0; 	// invalid match for sure
	return ((30.0*2.0)/distance);	// penalizing if distance is greater than 2.0 miles
}

/*
	Returns match value for Budget. Max. match value=30. 
	Match value decreases as difference from min or max budget increases.
	Returns -1.0 (invalid) if price shoots above or below 25% from the min or max budget.
*/
float cal_match_budget(long long price, long long min_budget, long long max_budget) {
	if(min_budget <= price && price <= max_budget) return 30.0;
	float difference_min = 100.0, difference_max = 100.0, difference;
	if(min_budget != -1) difference_min = 100.0*(abs(min_budget-price)/budget);
	if(max_budget != -1) difference_max = 100.0*(abs(max_budget-price)/budget);
	difference = min(difference_min, difference_max);
	if(difference > 25.0) return -1.0;	// invalid match for sure
	return (30.0-difference);	// penalizing for larger distance
}

/*
	Returns match value for bedrooms. Max. match value=20.
	Returns -1.0 (invalid) if number of bedrooms differ by more than 2 from min or max bedrooms.
*/
float cal_match_bedrooms(int bedrooms, int min_bedrooms, int max_bedrooms) {
	if(min_bedrooms <= bedrooms && bedrooms <= max_bedrooms) return 20.0;
	int difference_min = 3, difference_max = 3, difference;
	if(min_bedrooms != -1) difference_min = abs(min_bedrooms-bedrooms);
	if(max_bedrooms != -1) difference_max = abs(max_bedrooms-bedrooms);
	difference = min(difference_min, difference_max);
	if(difference > 2) return -1.0;		// invalid match for sure 
	if(difference == 1) return 10.0;	// Only two options left (1 or 2) as difference = 0 would have already matched
	if(difference == 2) return 5.0;
}

/*
	Returns match value for bathrooms. Max. match value=20.
	Returns -1.0 (invalid) if number of bathrooms differ by more than 2 from min or max bathrooms.
*/
float cal_match_bathrooms(int bathrooms, int min_bathrooms, int max_bathrooms) {
	if(min_bathrooms <= bathrooms && bathrooms <= max_bathrooms) return 20.0;
	int difference_min = 3, difference_max = 3, difference;
	if(min_bathrooms != -1) difference_min = abs(min_bathrooms-bathrooms);
	if(max_bathrooms != -1) difference_max = abs(max_bathrooms-bathrooms);
	difference = min(difference_min, difference_max);
	if(difference > 2) return -1.0;		// invalid match for sure 
	if(difference == 1) return 10.0;	// Only two options left (1 or 2) as difference =0 would have already matched
	if(difference == 2) return 5.0;
}

/*
	Comparison function which sorts the properties first on the basis of highest total match value
	then on the basis of match from the required distance if the total match for the properties are same,
	then on the basis of match from the required bedrooms if the total match and distance match for the properties are same,
	and finally on the basis of match from the required bathrooms if the total match, distance match and bedroom match for the properties are same.

*/
bool comparison(property L, property R) {
	if(L.first.first != R.first.first) return (L.first.first > R.first.first);
	if(L.first.second.first != R.first.second.first) return (L.first.second.first > R.first.second.first);
	return (L.first.second.second > R.first.second.second);
}

bool check_inputs(int id, double latitude, double longitude, long long min_budget, long long max_budget, int min_bedrooms, int max_bedrooms, int min_bathrooms, int max_bathrooms) {
	if(max(min_budget, max_budget) > 0 && max(min_bedrooms, max_bedrooms) > 0 && max(min_bathrooms, max_bathrooms) > 0) return true;  // valid input
	retun false;	// invalid input
}

/*
	------------------------------------THIS FUNCTION WILL BE CALLED BY MAIN-----------------------------------------

	Assumption: In case a value is not present, it is passed as -1 in the function.
	Parameters are passed according to the attributes of any property requirement.
	Returns a list of all the properties matching the requirements above 40%.   
*/
vector <property> get_matches(int id, double latitude, double longitude, long long min_budget, long long max_budget, int min_bedrooms, int max_bedrooms, int min_bathrooms, int max_bathrooms) {

	// vector <property> all_properties - is a list of all the properties fetched from the database. 
	bool validity = check_inputs(min_budget, max_budget, min_bedrooms, max_bedrooms, min_bathrooms, max_bathrooms);
	// Initially empty list of properties
	vector <property> matched_properties;		
	if(validity == 0) {
		// empty list is returned in case of invalid input
		return matched_properties;
	}
	/* 
		Empty list containing  shortlisted properties and match details: 
		<<Total_match, <Distance_match, Budget_match>>, Property_details>
		It is stored in this way because we need to do priority based sorting i.e.,
		in the results properties with highest total_match value will be shown first, 
		if the value of total_match is same for any two properties then the one with 
		lower mismatch in terms of distance will be shown first and if this value is also same,
		then the one with closer in terms of budget will be shown first.
	*/
	vector < pair<pair<float, pair<float, float>>, property> > unsorted_properties; 
	for(int i=0 ; i<all_properties.size() ; i++) {
		float match_radius = cal_match_radius(all_properties[i].latitude, all_properties[i].longitude, latitude, longitude);
		float match_budget = cal_match_budget(all_properties[i].price, min_budget, max_budget);
		float match_bedrooms = cal_match_bedrooms(all_properties[i].bedrooms, min_bedrooms, max_bedrooms);
		float match_bathrooms = cal_match_bathrooms(all_properties[i].bathrooms, min_bathrooms, max_bathrooms);

		float net_match = match_radius+match_budget+match_bedrooms+match_bathrooms;
		if(net_match >= 40.0) {
			unsorted_properties.push_back(make_pair(make_pair(net_match, make_pair(match_radius, match_budget)), all_properties[i]));
		}
	}
	// sorting the properties according to priority
	sort(unsorted_properties.begin(), unsorted_properties.end(), comparison());
	for(int i=0 ; i<unsorted_properties.size() ; i++) {
		matched_properties.push_back(unsorted_properties[i].second);
	}

	return matched_properties;					// List of all matched properties returned
}