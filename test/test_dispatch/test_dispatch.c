

#include <unity.h>
#include <scaffold.h>
#include <modbus/dispatch.h>

#include <modbus/diagnostics.h>

#include <modbus/fcodes/bits.h>
#include <modbus/fcodes/regs.h>
#include <modbus/fcodes/control.h>
#include <modbus/fcodes/eit.h>
#include <modbus/fcodes/special.h>

void test_dispatch_rd_coils(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_COILS);
    #if MB_SUPPORT_FC_RD_COILS
    TEST_ASSERT_EQUAL(&modbus_handler_rdbits, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rd_inputs(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_INPUTS);
    #if MB_SUPPORT_FC_RD_INPUTS
    TEST_ASSERT_EQUAL(&modbus_handler_rdbits, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rd_hreg(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_HREG);
    #if MB_SUPPORT_FC_RD_HREG
    TEST_ASSERT_EQUAL(&modbus_handler_rdregs, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rd_ireg(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_IREG);
    #if MB_SUPPORT_FC_RD_IREG
    TEST_ASSERT_EQUAL(&modbus_handler_rdregs, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_scoil(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_SCOIL);
    #if MB_SUPPORT_FC_WR_SCOIL
    TEST_ASSERT_EQUAL(&modbus_handler_wrsbit, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_sreg(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_SREG);
    #if MB_SUPPORT_FC_WR_SREG
    TEST_ASSERT_EQUAL(&modbus_handler_wrsreg, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}


void test_dispatch_wr_excst(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_EXCST);
    #if MB_SUPPORT_FC_RD_EXCST
    TEST_ASSERT_EQUAL(&modbus_handler_rdexcst, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}


void test_dispatch_diagnostics(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_DIAGNOSTICS);
    #if MB_SUPPORT_FC_DIAGNOSTICS
    TEST_ASSERT_EQUAL(&modbus_handler_diagnostics, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_gt_cecnt(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_GT_CECNT);
    #if MB_SUPPORT_FC_GT_CECNT
    TEST_ASSERT_EQUAL(&modbus_handler_gtcelog, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_gt_celog(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_GT_CELOG);
    #if MB_SUPPORT_FC_GT_CELOG
    TEST_ASSERT_EQUAL(&modbus_handler_gtcelog, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_mcoils(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_MCOILS);
    #if MB_SUPPORT_FC_WR_MCOILS
    TEST_ASSERT_EQUAL(&modbus_handler_wrbits, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_mregs(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_MREGS);
    #if MB_SUPPORT_FC_WR_MREGS
    TEST_ASSERT_EQUAL(&modbus_handler_wrregs, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rep_sid(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_REP_SID);
    #if MB_SUPPORT_FC_REP_SID
    TEST_ASSERT_EQUAL(&modbus_handler_repsid, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rd_frec(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_FREC);
    #if MB_SUPPORT_FC_RD_FREC
    TEST_ASSERT_EQUAL(&modbus_handler_rdfrec, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_frec(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_FREC);
    #if MB_SUPPORT_FC_WR_FREC
    TEST_ASSERT_EQUAL(&modbus_handler_wrfrec, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_wr_regm(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_WR_REGM);
    #if MB_SUPPORT_FC_WR_REGM
    TEST_ASSERT_EQUAL(&modbus_handler_wrregm, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rw_mregs(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RW_MREGS);
    #if MB_SUPPORT_FC_RW_MREGS
    TEST_ASSERT_EQUAL(&modbus_handler_rwmregs, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_rd_fifoq(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_RD_FIFOQ);
    #if MB_SUPPORT_FC_RD_FIFOQ
    TEST_ASSERT_EQUAL(&modbus_handler_fifoq, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_eit(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(MB_FC_EIT);
    #if MB_SUPPORT_FC_EIT
    TEST_ASSERT_EQUAL(&modbus_handler_eit, result->handler);
    #else
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
    #endif
}

void test_dispatch_null(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(0);
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
}

void test_dispatch_invalid(void) {
    const modbus_fcode_handler_t* result = modbus_get_fcode_handler(0x42);
    TEST_ASSERT_EQUAL(&modbus_handler_notimpl, result->handler);
}

int main( int argc, char **argv) {
    init();
    UNITY_BEGIN();
    RUN_TEST(test_dispatch_rd_coils);
    RUN_TEST(test_dispatch_rd_inputs);
    RUN_TEST(test_dispatch_rd_hreg);
    RUN_TEST(test_dispatch_rd_ireg);
    RUN_TEST(test_dispatch_wr_scoil);
    RUN_TEST(test_dispatch_wr_sreg);
    RUN_TEST(test_dispatch_wr_excst);
    RUN_TEST(test_dispatch_diagnostics);
    RUN_TEST(test_dispatch_gt_cecnt);
    RUN_TEST(test_dispatch_gt_celog);
    RUN_TEST(test_dispatch_wr_mcoils);
    RUN_TEST(test_dispatch_wr_mregs);
    RUN_TEST(test_dispatch_rep_sid);
    RUN_TEST(test_dispatch_rd_frec);
    RUN_TEST(test_dispatch_wr_frec);
    RUN_TEST(test_dispatch_wr_regm);
    RUN_TEST(test_dispatch_rw_mregs);
    RUN_TEST(test_dispatch_rd_fifoq);
    RUN_TEST(test_dispatch_eit);
    RUN_TEST(test_dispatch_null);
    RUN_TEST(test_dispatch_invalid);
    UNITY_END();
}
