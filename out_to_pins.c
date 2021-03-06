// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Based on generic Bluetooth SCO link
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/soc.h>

static const struct snd_soc_dapm_widget OutToPins_widgets[] = {
	SND_SOC_DAPM_INPUT("RX"),
	SND_SOC_DAPM_OUTPUT("TX"),
};

static const struct snd_soc_dapm_route OutToPins_routes[] = {
	{ "Capture", NULL, "RX" },
	{ "TX", NULL, "Playback" },
};

static struct snd_soc_dai_driver OutToPins_dai[] = {
	{
		.name = "OutToPins-pcm",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 1,
			.channels_max = 4,
 			.rates = SNDRV_PCM_RATE_CONTINUOUS,
 			/* DMA does not support 24 bits transfers */
 			.formats =
 				SNDRV_PCM_FMTBIT_S16_LE |
 				SNDRV_PCM_FMTBIT_S32_LE,
		},
		.capture = {
			 .stream_name = "Capture",
			.channels_min = 1,
			.channels_max = 4,
 			.rates = SNDRV_PCM_RATE_CONTINUOUS,
 			/* DMA does not support 24 bits transfers */
 			.formats =
 				SNDRV_PCM_FMTBIT_S16_LE |
 				SNDRV_PCM_FMTBIT_S32_LE,
		},
	},

};

static int OutToPins_of_xlate_dai_id(struct snd_soc_component *component,
				   struct device_node *endpoint)
{
	/* return dai id 0, whatever the endpoint index */
	return 0;
}

static const struct snd_soc_component_driver soc_component_dev_OutToPins = {
	.dapm_widgets		= OutToPins_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(OutToPins_widgets),
	.dapm_routes		= OutToPins_routes,
	.num_dapm_routes	= ARRAY_SIZE(OutToPins_routes),
	.of_xlate_dai_id	= OutToPins_of_xlate_dai_id,
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

static int OutToPins_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
				      &soc_component_dev_OutToPins,
				      OutToPins_dai, ARRAY_SIZE(OutToPins_dai));
}

static int OutToPins_remove(struct platform_device *pdev)
{
	return 0;
}


#if defined(CONFIG_OF)
static const struct of_device_id OutToPins_codec_of_match[] = {
	{ .compatible = "OutToPins_codec", },
	{},
};
MODULE_DEVICE_TABLE(of, OutToPins_codec_of_match);
#endif

static struct platform_driver OutToPins_driver = {
	.driver = {
		.name = "OutToPins_codec",
		.of_match_table = of_match_ptr(OutToPins_codec_of_match),
	},
	.probe = OutToPins_probe,
	.remove = OutToPins_remove,
};

module_platform_driver(OutToPins_driver);

MODULE_AUTHOR("martin.lesniak@st.com");
MODULE_DESCRIPTION("ASoC generic output driver");
MODULE_LICENSE("GPL");
