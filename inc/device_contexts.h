#ifndef DEVICE_CONTEXTS_H
#define DEVICE_CONTEXTS_H

#include <iommu_tests.h>

// Number of entries of the root DDT (4-kiB / 64 bytes p/ entry)
#define DDT_N_ENTRIES       (0x1000 / 64)   // 64 entries

// Mask for ddtp.PPN (ddtp[53:10])
#define DDTP_PPN_MASK    (0x3FFFFFFFFFFC00ULL)

// Sv39 encoding to configure DC.fsc (iosatp)
#define IOSATP_MODE_SV39    (0x8ULL << 60)
// Sv39x4 encoding to configure DC.iohgatp
#define IOHGATP_MODE_SV39X4 (0x8ULL << 60)
// Definitive mode
#if (IOSATP_BARE == 1)
#   define IOSATP_MODE          (0x0ULL)
#else
#   define IOSATP_MODE          (IOSATP_MODE_SV39)
#endif

#if (IOHGATP_BARE == 1)
#   define IOHGATP_MODE         (0x0ULL)
#else
#   define IOHGATP_MODE         (IOHGATP_MODE_SV39X4)
#endif

// MSI translation mode encoding to configure DC.msiptp
#define MSIPTP_MODE_FLAT    (0x1ULL << 60)

#if (MSIPTP_OFF == 1)
#   define MSIPTP_MODE         (0x0ULL)
#else
#   define MSIPTP_MODE         (MSIPTP_MODE_FLAT)
#endif

// MSI address mask and pattern
#define MSI_ADDR_MASK       (0x296ULL)   // ...0010_1001_0110
#define MSI_ADDR_PATTERN    (0x385ULL)   // ...0011_1000_0101
//  MSI GPA                  0x103ULL       ...0001_0000_0011

// Define DDT mode
#if (IOMMU_BARE == 1)
#   define DDT_MODE                 (~(0ULL-IOMMU_OFF) & 1ULL)
#else
#   if (DC_EXT_FORMAT == 1)
#      if (DEVICE_ID_WIDTH <= 6)
#         define DDT_MODE           (~(0ULL-IOMMU_OFF) & 2ULL)
#      else
#         if (DEVICE_ID_WIDTH <= 15)
#             define DDT_MODE       (~(0ULL-IOMMU_OFF) & 3ULL)
#         else
#             define DDT_MODE       (~(0ULL-IOMMU_OFF) & 4ULL)
#         endif
#      endif
#   else
#      if (DEVICE_ID_WIDTH <= 7)
#         define DDT_MODE           (~(0ULL-IOMMU_OFF) & 2ULL)
#      else
#         if (DEVICE_ID_WIDTH <= 16)
#             define DDT_MODE       (~(0ULL-IOMMU_OFF) & 3ULL)
#         else
#             define DDT_MODE       (~(0ULL-IOMMU_OFF) & 4ULL)
#         endif
#      endif
#   endif
#endif

// DC.tc flags
#define DC_TC_VALID     (1ULL << 0 )    // Valid
#define DC_TC_EN_ATS    (1ULL << 1 )    // EN PCIe Address Translation Services
#define DC_TC_EN_PRI    (1ULL << 2 )    // EN PCIe Page Request If
#define DC_TC_T2GPA     (1ULL << 3 )    // The IOMMU respond ATS Requests with GPAs
#define DC_TC_DTF       (1ULL << 4 )    // Disable report of translation faults through the FQ
#define DC_TC_PDTV      (1ULL << 5 )    // PC associated to this DC
#define DC_TC_PRPR      (1ULL << 6 )    // Include PASID in IOMMU-generated PRGR message if associated PR has a PASID
#define DC_TC_GADE      (1ULL << 7 )    // The IOMMU updates second-stage A and D bits atomically
#define DC_TC_SADE      (1ULL << 8 )    // The IOMMU updates first-stage A and D bits atomically
#define DC_TC_DPE       (1ULL << 9 )    // Default process_id value is 0 for requests without valid process_id
#define DC_TC_SBE       (1ULL << 10)    // Endianness for implicit accesses to PDT and first-stage PTEs
#define DC_TC_SXL       (1ULL << 11)    // To define first-stage translation scheme
#define DC_TC_RSV       (1ULL << 12)    // To raise fault for setting rsvd fields

// Device Context Indexes
enum test_dc {
    INVALID,
    TC_RSVD_SET,
    EN_PRI,
    T2GPA,
    SADE,
    SBE,
    SXL,
    BASIC,
    DISABLE_TF,
    DC_TOP = (DDT_N_ENTRIES-1),
    TEST_DC_MAX
};

void ddt_init(void);

#endif  /* DEVICE_CONTEXTS_H */