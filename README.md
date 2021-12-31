# OutToPins-alsa-codec
Simple ALSA codec which allows direct audio output (I2S or TDM) from MP1 SAI or SPI/I2S ports 

The driver can be built stand-alone or added to the Linux sources. 
The following device tree entries are required for proper operation (SAI2 example):

root entry:

OutToPins_codec: OutToPins_codec@0 {
	 compatible = "OutToPins_codec";
	 #reg = <0>;
	 #sound-dai-cells = <0>;
	 clocks = <&sai2a>;
	 clock-names = "MCLK";
	 status = "okay";

	 	OutToPins_port: port {
		#address-cells = <1>;
		#size-cells = <0>;

		OutToPins_tx_endpoint: endpoint@0 {
			reg = <0>;
			remote-endpoint = <&sai2a_endpoint>;
			//frame-master;     /* uncomment if an external master provides this signal -> FrameSync */  
			//bitclock-master;  /* uncomment if an external master provides this signal -> Bitclock */ 
		};

		OutToPins_rx_endpoint: endpoint@1 {
			reg = <1>;
			remote-endpoint = <&sai2b_endpoint>;
			//frame-master;     /* uncomment if an external master provides this signal -> FrameSync */
			//bitclock-master;  /* uncomment if an external master provides this signal -> Bitclock */
		};
	};
};

SAI2 entry:

&sai2 {
	clocks = <&rcc SAI2>, <&rcc PLL3_Q>, <&rcc PLL3_R>;
	clock-names = "pclk", "x8k", "x11k";
	pinctrl-names = "default", "sleep";
	pinctrl-0 = <&sai2a_pins_a>, <&sai2b_pins_b>;
	pinctrl-1 = <&sai2a_sleep_pins_a>, <&sai2b_sleep_pins_b>;
	status = "okay";

	sai2a: audio-controller@4400b004 {
		#clock-cells = <0>;
		dma-names = "tx";
		status = "okay";

		sai2a_port: port {
			sai2a_endpoint: endpoint {
			remote-endpoint = <&OutToPins_tx_endpoint>;
			format = "i2s";
			mclk-fs = <256>;
			};
		};
	};

	sai2b: audio-controller@4400b024 {
		dma-names = "rx";
		st,sync = <&sai2a 2>;
		clocks = <&rcc SAI2_K>, <&sai2a>;
		clock-names = "sai_ck", "MCLK";
		status = "okay";

		sai2b_port: port {
			sai2b_endpoint: endpoint {
			remote-endpoint = <&OutToPins_rx_endpoint>;
			format = "i2s";
			mclk-fs = <256>;
			};
		};
	};
};
