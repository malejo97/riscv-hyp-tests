/**
 * !NOTES:
 * 
 *  -   
 */

#include <fault_queue.h>

// N_entries * 32 bytes
uint64_t fault_queue[FQ_N_ENTRIES * 4 * sizeof(uint64_t)] __attribute__((aligned(PAGE_SIZE)));

void fq_init()
{
    uint64_t   fqb;
    uintptr_t  fqb_addr    = IOMMU_REG_ADDR(IOMMU_FQB_OFFSET);
    uintptr_t  fqh_addr    = IOMMU_REG_ADDR(IOMMU_FQH_OFFSET);
    uintptr_t  fqcsr_addr  = IOMMU_REG_ADDR(IOMMU_FQCSR_OFFSET);

    // Configure fqb with base PPN of the queue and size as log2(N)
    fqb = ((((uintptr_t)fault_queue) >> 2) & FQB_PPN_MASK) | FQ_LOG2SZ_1;
    write64(fqb_addr, fqb);

    // Set fqh to zero
    write32(fqh_addr, 0);

    // Write 1 to fqcsr.fqen to enable the FQ
    // INFO: Interrupts disabled
    write32(fqcsr_addr, FQCSR_FQEN);

    // Poll fqcsr.fqon until it reads 1
    while (!(read32(fqcsr_addr) & FQCSR_FQON));
}