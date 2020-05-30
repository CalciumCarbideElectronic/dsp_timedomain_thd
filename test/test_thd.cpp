#include "gtest/gtest.h"
extern "C" {
#include "thd.h"
}
TEST(main,ok){
    ASSERT_EQ(12345,_calculate_rms(NULL,NULL)); }




