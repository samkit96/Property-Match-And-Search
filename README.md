# Property-Match-And-Search
Algorithm to perform priority based matching of a required property and return a list of matching properties.

Assumptions:
1. Functionalities for inserting and reading records from database is present.
2. In case a value is not present, it is passed as -1 in the method get_matches.
3. Latitude and longitude are passed in radians in the method get_matches.

The input parameters for the method get_matches() are the requirements. It returns a list of proprties sorted in a way that
the most matched property appears at the top and the least matched at the bottom. A property is added in the returned list only
if the total match with requirements of that property is greater than 40% and the distance fromn requirement is within 10 miles, 
the budget from requirement is +/- 25%, and bedrooms and bathrooms from requirement are +/- 2. Initially the properties are not 
sorted and are stored in a vector containing net match, match for distance and match for bathrooms along with property details.
Finally after sorting this unsorted properties, the properties are stored in vector mateched_properties which is then returned.

Method check_inputs() return false in case of invalid inputs and true in case of valid inputs.

Method cal_match_radius returns the match percent out of 30 for properties on the basis of distance. 
The match percent decreases as distance increases.
The distance between two points whose latitude and longitude are known is calculated using Haversine formula.
If distance is greater than 10 miles, it returns zero. 
If distance is less than 2 miles, it is invalid.
Otherwise it returns (60/distance).

Method cal_match_budget returns the match percent out of 30 for properties on the basis of budget. 
The match percent decreases as variation from budget increases.
If variation is greater than 25%, it is invalid. 
If variation is 0% than it return 30.
Otherwise it returns (30-variation).

Method cal_match_bedrooms returns the match percent out of 20 for properties on the basis of number of bedrooms. 
The match percent decreases as difference from required bedrooms increases.
If bedrooms are between min_bedrooms and max_bedrooms, it returns 20. 
If difference is greater than 2 bedrooms, it is invalid. 
If difference is equal to 2 bedrooms, it returns match as 5.
If difference is equal to 1 bedroom, it returns match as 10.

Method cal_match_bathrooms returns the match percent out of 20 for properties on the basis of number of bathrooms. 
The match percent decreases as difference from required bathrooms increases.
If bathrooms are between min_bathrooms and max_bathrooms, it returns 20. 
If difference is greater than 2 bathrooms, it is invalid. 
If difference is equal to 2 bathrooms, it returns match as 5.
If difference is equal to 1 bathrooms, it returns match as 10.

The comparison function sorts the properties first on the basis of highest total match value
then on the basis of match from the required distance if the total match for the properties are same,
then on the basis of match from the required bedrooms if the total match and distance match for the properties are same,
and finally on the basis of match from the required bathrooms if the total match, distance match and bedroom match for the 
properties are same.
