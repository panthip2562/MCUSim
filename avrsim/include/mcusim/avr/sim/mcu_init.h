/*
 * AVRSim - Simulator for AVR microcontrollers.
 * This software is a part of MCUSim, interactive simulator for
 * microcontrollers.
 * Copyright (C) 2017 Dmitry Salychev <darkness.bsd@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

unsigned long dmsz, pmsz;

if (!mcu) {
	fprintf(stderr, "MCU should not be NULL\n");
	return -1;
}
strcpy(mcu->name, MCU_NAME);

mcu->signature[0] = SIGNATURE_0;
mcu->signature[1] = SIGNATURE_1;
mcu->signature[2] = SIGNATURE_2;
mcu->spm_pagesize = SPM_PAGESIZE;
mcu->flashstart = FLASHSTART;
mcu->flashend = FLASHEND;
mcu->ramstart = RAMSTART;
mcu->ramend = RAMEND;
mcu->ramsize = RAMSIZE;
mcu->e2start = E2START;
mcu->e2end = E2END;
mcu->e2size = E2SIZE;
mcu->e2pagesize = E2PAGESIZE;

srand((unsigned int) time(NULL));

mcu->id = (uint32_t) rand();
mcu->lockbits = LBITS_DEFAULT;
mcu->sfr_off = __SFR_OFFSET;
mcu->regs = GP_REGS;
mcu->io_regs = IO_REGS;

/* Program memory */
pmsz = (mcu->flashend - mcu->flashstart) + 1;
if (pm_size < pmsz) {
	fprintf(stderr, "Program memory is limited by %lu bytes,"
			" %lu bytes isn't enough\n",
			pmsz, pm_size);
	return -1;
}
mcu->pm = pm;
mcu->pm_size = pm_size;
/* END Program memory */

/* Data memory */
dmsz = mcu->regs + mcu->io_regs + mcu->ramsize;
if (dm_size < dmsz) {
	fprintf(stderr, "Data memory is limited by %lu bytes,"
			" %lu bytes isn't enough\n",
			dmsz, dm_size);
	return -1;
}
mcu->dm = dm;
mcu->dm_size = dm_size;
/* END Data memory */

mcu->sreg = &mcu->dm[SREG];
mcu->sph = &mcu->dm[SPH];
mcu->spl = &mcu->dm[SPL];

/* Extended registers */
#ifdef EIND
mcu->eind = &mcu->dm[_SFR_IO8(EIND)];
#else
mcu->eind = NULL;
#endif
#ifdef RAMPZ
mcu->rampz = &mcu->dm[_SFR_IO8(RAMPZ)];
#else
mcu->rampz = NULL;
#endif
#ifdef RAMPY
mcu->rampy = &mcu->dm[_SFR_IO8(RAMPY)];
#else
mcu->rampy = NULL;
#endif
#ifdef RAMPX
mcu->rampx = &mcu->dm[_SFR_IO8(RAMPX)];
#else
mcu->rampx = NULL;
#endif
#ifdef RAMPD
mcu->rampd = &mcu->dm[_SFR_IO8(RAMPD)];
#else
mcu->rampd = NULL;
#endif
/* END Extended registers */

/* Fuses */
#ifdef EFUSE_DEFAULT
mcu->fuse[2] = EFUSE_DEFAULT;
#endif
#ifdef HFUSE_DEFAULT
mcu->fuse[1] = HFUSE_DEFAULT;
#endif
mcu->fuse[0] = LFUSE_DEFAULT;

#ifdef SET_FUSE_FUNC
mcu->set_fusef = SET_FUSE_FUNC;
#else
mcu->set_fusef = NULL;
#endif
#ifdef SET_LOCK_FUNC
mcu->set_lockf = SET_LOCK_FUNC;
#else
mcu->set_lockf = NULL;
#endif
/* END Fuses */

#ifdef BLS_START
mcu->bls->start = BLS_START;
mcu->bls->end = BLS_END;
mcu->bls->size = BLS_SIZE;
#else
mcu->bls->start = 0;
mcu->bls->end = 0;
mcu->bls->size = 0;
#endif

#if defined(SPMCSR)
mcu->spmcsr = &mcu->dm[_SFR_IO8(SPMCSR)];
#elif defined(SPMCR)
mcu->spmcsr = &mcu->dm[_SFR_IO8(SPMCR)];
#else
mcu->spmcsr = NULL;
#endif

mcu->clk_source = CLK_SOURCE;
mcu->freq = CLK_FREQ;
mcu->reset_pc = RESET_PC;
mcu->pc = mcu->reset_pc;
mcu->pc_bits = PC_BITS;

return 0;