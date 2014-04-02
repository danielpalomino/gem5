/*
 * Copyright (c) 2004-2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ali Saidi
 *          Andrew Schultz
 *          Nathan Binkert
 */

/* @file
 * Interface for devices using PCI configuration
 */

#ifndef __DEV_PCIDEV_HH__
#define __DEV_PCIDEV_HH__

#include <cstring>

#include "dev/dma_device.hh"
#include "dev/pcireg.h"
#include "dev/platform.hh"
#include "params/PciDevice.hh"
#include "sim/byteswap.hh"

#define BAR_IO_MASK 0x3
#define BAR_MEM_MASK 0xF
#define BAR_IO_SPACE_BIT 0x1
#define BAR_IO_SPACE(x) ((x) & BAR_IO_SPACE_BIT)
#define BAR_NUMBER(x) (((x) - PCI0_BASE_ADDR0) >> 0x2);



/**
 * PCI device, base implementation is only config space.
 */
class PciDev : public DmaDevice
{
    class PciConfigPort : public SimpleTimingPort
    {
      protected:
        PciDev *device;

        virtual Tick recvAtomic(PacketPtr pkt);

        virtual AddrRangeList getAddrRanges() const;

        Platform *platform;

        int busId;
        int deviceId;
        int functionId;

        Addr configAddr;

      public:
        PciConfigPort(PciDev *dev, int busid, int devid, int funcid,
                      Platform *p);
    };

  public:
    typedef PciDeviceParams Params;
    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }

  protected:
    /** The current config space.  */
    PCIConfig config;

    /** The size of the BARs */
    uint32_t BARSize[6];

    /** The current address mapping of the BARs */
    Addr BARAddrs[6];

    /** Whether the BARs are really hardwired legacy IO locations. */
    bool legacyIO[6];

    /**
     * Does the given address lie within the space mapped by the given
     * base address register?
     */
    bool
    isBAR(Addr addr, int bar) const
    {
        assert(bar >= 0 && bar < 6);
        return BARAddrs[bar] <= addr && addr < BARAddrs[bar] + BARSize[bar];
    }

    /**
     * Which base address register (if any) maps the given address?
     * @return The BAR number (0-5 inclusive), or -1 if none.
     */
    int
    getBAR(Addr addr)
    {
        for (int i = 0; i <= 5; ++i)
            if (isBAR(addr, i))
                return i;

        return -1;
    }

    /**
     * Which base address register (if any) maps the given address?
     * @param addr The address to check.
     * @retval bar The BAR number (0-5 inclusive),
     *             only valid if return value is true.
     * @retval offs The offset from the base address,
     *              only valid if return value is true.
     * @return True iff address maps to a base address register's region.
     */
    bool
    getBAR(Addr addr, int &bar, Addr &offs)
    {
        int b = getBAR(addr);
        if (b < 0)
            return false;

        offs = addr - BARAddrs[b];
        bar = b;
        return true;
    }

  protected:
    Platform *platform;
    Tick pioDelay;
    Tick configDelay;
    PciConfigPort configPort;

    /**
     * Write to the PCI config space data that is stored locally. This may be
     * overridden by the device but at some point it will eventually call this
     * for normal operations that it does not need to override.
     * @param pkt packet containing the write the offset into config space
     */
    virtual Tick writeConfig(PacketPtr pkt);


    /**
     * Read from the PCI config space data that is stored locally. This may be
     * overridden by the device but at some point it will eventually call this
     * for normal operations that it does not need to override.
     * @param pkt packet containing the write the offset into config space
     */
    virtual Tick readConfig(PacketPtr pkt);

  public:
    Addr pciToDma(Addr pciAddr) const
    { return platform->pciToDma(pciAddr); }

    void
    intrPost()
    { platform->postPciInt(letoh(config.interruptLine)); }

    void
    intrClear()
    { platform->clearPciInt(letoh(config.interruptLine)); }

    uint8_t
    interruptLine()
    { return letoh(config.interruptLine); }

    /**
     * Determine the address ranges that this device responds to.
     *
     * @return a list of non-overlapping address ranges
     */
    AddrRangeList getAddrRanges() const;

    /**
     * Constructor for PCI Dev. This function copies data from the
     * config file object PCIConfigData and registers the device with
     * a PciConfigAll object.
     */
    PciDev(const Params *params);

    virtual void init();

    /**
     * Serialize this object to the given output stream.
     * @param os The stream to serialize to.
     */
    virtual void serialize(std::ostream &os);

    /**
     * Reconstruct the state of this object from a checkpoint.
     * @param cp The checkpoint use.
     * @param section The section name of this object
     */
    virtual void unserialize(Checkpoint *cp, const std::string &section);


    virtual unsigned int drain(Event *de);

    virtual SlavePort &getSlavePort(const std::string &if_name, int idx = -1)
    {
        if (if_name == "config") {
            return configPort;
        }
        return DmaDevice::getSlavePort(if_name, idx);
    }

};
#endif // __DEV_PCIDEV_HH__
