# My Email: Ahmad25.2.2002@gmail.com

# Steam Tables:
## Author of the .csv files of steam tables: Neil Hendren
## License of steam tables files:
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
## Copyright:
2022 University of Colorado Boulder
## Comments from the website of authors of steam tables
- The data in these steam tables is provided by the National Institute of Standards and Technology (NIST) and is based on formulations by the International Association for the Properties of Water and Steam (IAPWS).
- The Excel steam tables are updated regularly to improve usability and eliminate bugs, so download the most recent version.
- Current version of superheated and compressed steam tables: v1.3.
- Current version of saturated by temperature and saturated by pressure tables: v1.4.
- You can find these steam tables at LearnChemE website: *[LearnChemE website](https://learncheme.com/student-resources/steam-tables/)*

# R134 Tables:
## They are not yet implemented in a way that can be used in the program
## Downloaded from [Ohio University](https://www.ohio.edu/mechanical/thermo/property_tables/R134a/)
## Source of Data: [NIST Chemistry WebBook](http://webbook.nist.gov/chemistry/fluid/) - accessed Jan 2008

# Some stuff that should be modified in the code:
1. Extrapolation for values less than or greater than the existing in the tables.
2. Display the phase of the fluid after fixing its state.
3. Are we able to fix the state if both inputs are rather than temperature or pressure?
4. Reheat
5. Recool
6. Feedwater heater
7. 
