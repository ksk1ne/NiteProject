#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/init/boot.h>
#include <siberix/mm/page.h>
#include <utils/alignment.h>

SegAlloc::SegAlloc() {
    BootConfig&    bootConfig    = exec()->getBootConfig();
    MemoryService& memoryService = exec()->getMemory();
    PageBlock*     blocks        = &bootConfig.memory.ranges[0];

    for (int i = 0; i < 256; i++) {
        PageBlock& block = blocks[i];
        alignUpRef(block.start, static_cast<u64>(PAGE_SIZE_4K));
        alignDownRef(block.end, static_cast<u64>(PAGE_SIZE_4K));
        if (block.type != BlkTypeAvailable || (block.end - block.start < PAGE_SIZE_4K)) {
            continue;
        }

        getSegments()->add(block);
    }

    u64 current = 0;
    while (current < bootConfig.memory.maxSize) {
        u64 phys = this->allocatePhysMemory4K(SECTION_PAGE_SIZE / PAGE_SIZE_4K)->address;
        u64 virt = memoryService.allocVirtMemory4KPages(SECTION_PAGE_SIZE / PAGE_SIZE_4K);
        if (!(phys && virt)) {
            Logger::getAnonymousLogger().error("Cannot allocate memory in startup environment.");
            return;
        }

        Pageframe* pages                                = reinterpret_cast<Pageframe*>(virt);
        exec()->getMemory().getPageSect(current).pages  = reinterpret_cast<u64*>(pages->address);
        current                                        += PAGE_SIZE_1G;
    }
}

SegAlloc::~SegAlloc() {}

Pageframe* SegAlloc::allocatePhysMemory4K(u64 amount) {
    u64 address = 0;
    int i       = 0;
    while (!address && i < 256) {
        PageBlock& block = exec()->getMemory().getPageBlock(i);
        if (block.end - block.start > (amount * PAGE_SIZE_4K)) {
            address      = block.start;
            block.start += (amount * PAGE_SIZE_4K);
            break;
        }
    }
    Pageframe* page = addr2page(address);
    if (!page->address) {
        page->address = address;
    }
    return page;
}

void SegAlloc::freePhysMemory4K(u64 address) {}

void SegAlloc::freePhysMemory4K(Pageframe* page) {}

void SegAlloc::addSegment(u64 start, u64 end, PageBlockType type) {
    this->segments.add(PageBlock(start, end, type));
}