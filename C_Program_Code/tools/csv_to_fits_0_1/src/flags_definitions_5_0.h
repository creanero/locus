#ifndef FLAGS_DEFINITIONS_5_0_H_
#define FLAGS_DEFINITIONS_5_0_H_

#define GOOD_STATUS 0x2000 //SDSS Status = Primary (2000)
#define BAD_STATUS 0 //No bad flags defined
#define GOOD_OBJC_FLAGS 0x10000000 
#define BAD_OBJC_FLAGS 0x80020 //PEAKCENTRE || NOTCHECKED
#define GOOD_OBJC_FLAGS2 0x0 //little worried.  Can't seem to find a good flags2
#define BAD_OBJC_FLAGS2 0x8100 //PSFFLUXINTERP || BADCOUNTSERROR 
#define GOOD_OBJC_TYPE 6 //SDSS Star-Galaxty separation, used with objc_type

//Probably want to do a bit more than this.  Currently set up solely for tests.


#endif /*FLAGS_DEFINITIONS_5_0_H_*/
