// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
#ifndef MAME_EMU_EMUMEM_HEU_H
#define MAME_EMU_EMUMEM_HEU_H

#pragma once

// handler_entry_read_units/handler_entry_write_units

// merges/splits an access among multiple handlers (unitmask support)

template<int Width, int AddrShift> class handler_entry_read_units : public handler_entry_read<Width, AddrShift>
{
public:
	using uX = typename emu::detail::handler_entry_size<Width>::uX;

	handler_entry_read_units(const memory_units_descriptor<Width, AddrShift> &descriptor, u8 ukey, address_space *space);
	handler_entry_read_units(const memory_units_descriptor<Width, AddrShift> &descriptor, u8 ukey, const handler_entry_read_units *src);
	handler_entry_read_units(const handler_entry_read_units *src);
	~handler_entry_read_units();

	uX read(offs_t offset, uX mem_mask) const override;
	uX read_interruptible(offs_t offset, uX mem_mask) const override;
	std::pair<uX, u16> read_flags(offs_t offset, uX mem_mask) const override;
	u16 lookup_flags(offs_t offset, uX mem_mask) const override;

	std::string name() const override;

	void enumerate_references(handler_entry::reflist &refs) const override;
	handler_entry_read<Width, AddrShift> *dup() override;

private:
	static constexpr u32 SUBUNIT_COUNT = 1 << Width;

	struct subunit_info
	{
		handler_entry   *m_handler;              // the handler itself, the root to hide the templatization

		uX               m_amask;                // access mask (for filtering access)
		uX               m_dmask;                // data mask (for removal on collisions)
		s8               m_ashift;               // shift to apply to the address (positive = right)
		u8               m_offset;               // offset to add to the address post shift
		u8               m_dshift;               // data shift of the subunit

		u8               m_width;                // access width (0..3)
	};

	subunit_info         m_subunit_infos[SUBUNIT_COUNT]; // subunit information
	uX                   m_unmap;                        // "unmapped" value to add to reads
	u8                   m_subunits;                     // number of subunits

	void fill(const memory_units_descriptor<Width, AddrShift> &descriptor, const std::vector<typename memory_units_descriptor<Width, AddrShift>::entry> &entries);
	static std::string m2r(uX mask);
};

template<int Width, int AddrShift> class handler_entry_write_units : public handler_entry_write<Width, AddrShift>
{
public:
	using uX = typename emu::detail::handler_entry_size<Width>::uX;

	handler_entry_write_units(const memory_units_descriptor<Width, AddrShift> &descriptor, u8 ukey, address_space *space);
	handler_entry_write_units(const memory_units_descriptor<Width, AddrShift> &descriptor, u8 ukey, const handler_entry_write_units<Width, AddrShift> *src);
	handler_entry_write_units(const handler_entry_write_units *src);
	~handler_entry_write_units();

	void write(offs_t offset, uX data, uX mem_mask) const override;
	void write_interruptible(offs_t offset, uX data, uX mem_mask) const override;
	u16 write_flags(offs_t offset, uX data, uX mem_mask) const override;
	u16 lookup_flags(offs_t offset, uX mem_mask) const override;

	std::string name() const override;

	void enumerate_references(handler_entry::reflist &refs) const override;
	handler_entry_write<Width, AddrShift> *dup() override;

private:
	static constexpr u32 SUBUNIT_COUNT = 1 << Width;

	struct subunit_info
	{
		handler_entry   *m_handler;              // the handler itself, the root to hide the templatization

		uX               m_amask;                // access mask (for filtering access)
		uX               m_dmask;                // data mask (for removal on collisions)
		s8               m_ashift;               // shift to apply to the address (positive = right)
		u8               m_offset;               // offset to add to the address post shift
		u8               m_dshift;               // data shift of the subunit

		u8               m_width;                // access width (0..3)
	};

	subunit_info         m_subunit_infos[SUBUNIT_COUNT]; // subunit information
	u8                   m_subunits;                     // number of subunits

	void fill(const memory_units_descriptor<Width, AddrShift> &descriptor, const std::vector<typename memory_units_descriptor<Width, AddrShift>::entry> &entries);
	static std::string m2r(uX mask);
};

#endif // MAME_EMU_EMUMEM_HEU_H
