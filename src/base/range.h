#ifndef _SIRIUS_RANGE_H_
#define _SIRIUS_RANGE_H_

#define range_r1_include_r2_m(r1s, r1e, r2s, r2e) (r1s <= r2s && r1e >= r2e)
#define range_include_m(r1s, r1e, r2s, r2e) ( range_r1_include_r2_m(r1s, r1e, r2s, r2e) || range_r1_include_r2_m(r2s, r2e, r1s, r1e) )

#endif /* _SIRIUS_RANGE_H_ */

