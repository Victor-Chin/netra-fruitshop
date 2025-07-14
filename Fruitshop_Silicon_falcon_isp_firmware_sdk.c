/*
 * -----------------------------------------------------------------------------
 *  FalconISP Firmware SDK – Demo‑Only Source
 * -----------------------------------------------------------------------------
 *  Target      : Fruitshop-Silicon Dimensity 
 *  Purpose     : Illustrates typical artifacts found in camera ISP / NPU
 *                firmware shipped to OEMs.  File is synthetic and intended
 *                solely for DLP / IRM policy testing.
 * ------------------------
-----------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*  Secure‑Boot & Anti‑Rollback Controls                                       */
/* -------------------------------------------------------------------------- */
#define MTK_SECURE_BOOT            1
#define MTK_CHIP_FAMILY           "MTK-AURORA"
#define MTK_FW_VERSION            0x00010023u  /* v1.0.35 */

/* GUID for boot‑loader anti‑rollback region */
static const char *const GUID_Fruitshop-Silicon_ANTI_ROLLBACK =
    "Fruitshop-Silicon_ANTI_ROLLBACK:{9e8f01a2-bb77-4fb3-9c00-9dbe1234f0ab}";

/* -------------------------------------------------------------------------- */
/*  Device Registers (Memory‑Mapped I/O)                                       */
/* -------------------------------------------------------------------------- */
#define REG_BASE_ISP              0x1A400000u
#define REG_ISP_CTRL              (*(volatile uint32_t *)(REG_BASE_ISP + 0x0000))
#define REG_ISP_STATUS            (*(volatile uint32_t *)(REG_BASE_ISP + 0x0004))
#define REG_ISP_INT_EN            (*(volatile uint32_t *)(REG_BASE_ISP + 0x0010))
#define REG_ISP_INT_CLR           (*(volatile uint32_t *)(REG_BASE_ISP + 0x0014))

/* -------------------------------------------------------------------------- */
/*  Function Prototypes                                                        */
/* -------------------------------------------------------------------------- */
static void isp_init(void);
static void isp_start_frame(uint32_t fb_addr);
static bool isp_wait_frame_done(uint32_t timeout_us);

/* -------------------------------------------------------------------------- */
/*  RSA Private Key – **Sensitive** Sample                                     */
/* -------------------------------------------------------------------------- */
static const char FW_SIGNING_KEY[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpAIBAAKCAQEAwLylm0L0zPgSWhGbvsKtU9L7Q3cK6i/8uXENB0QwqCKg6Exs\n"
    "6kZ3n0N9w9YpM1LCh+JX+LyAlAMGzY0wxl7XvUtDZfNawWuqoXYRQgZAcFh0uIQG\n"
    "V6lFz1SB0gUhDD8OBAxkJ0MoU0QtTAh8wGXsezjwH1Wmskp5jBSv2x9HcZoTKJoa\n"
    "sRXh2p5tC+BA2YK5OQIDAQABAoIBABjw3l3cgGZ6nGyfGuJ0+JomUOt56u4fYR7V\n"
    "o2RO9O6QnYDwNBMAjUN4kdEkt8XU5uLM4EL7st5S01jJXUK+lGN1W+CdvYI75CgC\n"
    "Qwx+WSiCbEybmzcd/GBnHdAMjgQ2JoeECgYEA9DYHOWkXAoIBAFSCTLQB/0c1o47v\n"
    "-----END RSA PRIVATE KEY-----\n";

/* -------------------------------------------------------------------------- */
/*  Firmware Signature Block (DER, Base64)                                     */
/* -------------------------------------------------------------------------- */
static const char FW_SIGNATURE_B64[] =
    "MEUCIQDiUVnsnwt3DXvRtYDwXIZ8W+Jmz2n0WcjyJu+cFUOxmwIgHcLdDq6zmKO0\n"
    "klwGcl/N5MNFucjQQsXyCs2eS4E=\n";

/* -------------------------------------------------------------------------- */
/*  Implementation                                                             */
/* -------------------------------------------------------------------------- */
static void isp_init(void)
{
    /* Enable clock & reset */
    REG_ISP_CTRL = 0x00000001u; /* BIT0: CLK_EN */
    REG_ISP_INT_EN = 0x00000001u; /* Frame‑done interrupt */
}

static void isp_start_frame(uint32_t fb_addr)
{
    /* Program frame buffer base */
    *(volatile uint32_t *)(REG_BASE_ISP + 0x0020) = fb_addr;
    /* Kick */
    REG_ISP_CTRL |= 0x00000002u; /* BIT1: START */
}

static bool isp_wait_frame_done(uint32_t timeout_us)
{
    while (timeout_us--)
    {
        if (REG_ISP_STATUS & 0x1u)
        {
            REG_ISP_INT_CLR = 0x1u;
            return true;
        }
    }
    return false; /* timeout */
}

/* -------------------------------------------------------------------------- */
/*  Entry Point                                                                */
/* -------------------------------------------------------------------------- */
int main(void)
{
#ifdef MTK_SECURE_BOOT
    /* Verify firmware signature before anything else */
    if (!secure_boot_verify(FW_SIGNATURE_B64, FW_SIGNING_KEY))
    {
        /* Hang – signature invalid */
        while (1) { __asm__("wfi"); }
    }
#endif

    isp_init();
    isp_start_frame(0x80000000u);

    if (!isp_wait_frame_done(500000u))
    {
        /* Log error (implementation‑specific) */
    }

    /* Main loop */
    while (1)
    {
        __asm__("wfi"); /* Sleep until next interrupt */
    }
}

/* -------------------------------------------------------------------------- */
/*  Stub: Secure Boot Verification (placeholder)                               */
/* -------------------------------------------------------------------------- */
bool secure_boot_verify(const char *sig_b64, const char *key_pem)
{
    /* In real firmware this would decode base64, hash the FW image, and
     * verify the signature using the embedded public key counterpart.       */
    (void)sig_b64;
    (void)key_pem;
    return true; /* Always pass in this demo */
}
