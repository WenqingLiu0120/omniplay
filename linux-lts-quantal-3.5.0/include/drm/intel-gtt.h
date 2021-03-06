/* Common header for intel-gtt.ko and i915.ko */

#ifndef _DRM_INTEL_GTT_H
#define	_DRM_INTEL_GTT_H

struct intel_gtt {
	/* Size of memory reserved for graphics by the BIOS */
	unsigned int stolen_size;
	/* Total number of gtt entries. */
	unsigned int gtt_total_entries;
	/* Part of the gtt that is mappable by the cpu, for those chips where
	 * this is not the full gtt. */
	unsigned int gtt_mappable_entries;
	/* Whether i915 needs to use the dmar apis or not. */
	unsigned int needs_dmar : 1;
	/* Whether we idle the gpu before mapping/unmapping */
	unsigned int do_idle_maps : 1;
	/* Share the scratch page dma with ppgtts. */
	dma_addr_t scratch_page_dma;
	struct page *scratch_page;
	/* for ppgtt PDE access */
	u32 __iomem *gtt;
	/* needed for ioremap in drm/i915 */
	phys_addr_t gma_bus_addr;
} *intel_gtt_get(void);

int intel_gmch_probe(struct pci_dev *bridge_pdev, struct pci_dev *gpu_pdev,
		     struct agp_bridge_data *bridge);
void intel_gmch_remove(void);

bool intel_enable_gtt(void);

void intel_gtt_chipset_flush(void);
void intel_gtt_unmap_memory(struct scatterlist *sg_list, int num_sg);
void intel_gtt_clear_range(unsigned int first_entry, unsigned int num_entries);
int intel_gtt_map_memory(struct page **pages, unsigned int num_entries,
			 struct scatterlist **sg_list, int *num_sg);
void intel_gtt_insert_sg_entries(struct scatterlist *sg_list,
				 unsigned int sg_len,
				 unsigned int pg_start,
				 unsigned int flags);
void intel_gtt_insert_sg_entries_hsw(struct sg_table *st,
                                 unsigned int pg_start,
                                 unsigned int flags);
void intel_gtt_insert_pages(unsigned int first_entry, unsigned int num_entries,
			    struct page **pages, unsigned int flags);

/* Special gtt memory types */
#define AGP_DCACHE_MEMORY	1
#define AGP_PHYS_MEMORY		2

/* New caching attributes for gen6/sandybridge */
#define AGP_USER_CACHED_MEMORY_LLC_MLC (AGP_USER_TYPES + 2)
#define AGP_USER_UNCACHED_MEMORY (AGP_USER_TYPES + 4)

/* flag for GFDT type */
#define AGP_USER_CACHED_MEMORY_GFDT (1 << 3)

#ifdef CONFIG_INTEL_IOMMU
extern int intel_iommu_gfx_mapped;
#endif

#endif
