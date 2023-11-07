<template>
	<view class="controlPad-container">
		<button class="test" @touchstart="handleButtonDown('boardled.01')"
			@touchend="handleButtonUp('boardled.01')"
			@touchcancel="handleButtonUp('boardled.01')">test1</button>
		<button class="test" @touchstart="handleButtonDown('boardled.00')"
			@touchend="handleButtonUp('boardled.00')"
			@touchcancel="handleButtonUp('boardled.00')">test0</button>

		<!-- direct -->
		<view class="direct-container">
			<view class="circle-btn" @touchstart="handleButtonDown('direct.01')"
				@touchend="handleButtonUp('direct.01')"
				@touchcancel="handleButtonUp('direct.01')">▲</view>
			<view class="circle-btn" @touchstart="handleButtonDown('direct.03')"
				@touchend="handleButtonUp('direct.03')"
				@touchcancel="handleButtonUp('direct.03')">◀</view>
			<view class="circle-btn" @touchstart="handleButtonDown('direct.04')"
				@touchend="handleButtonUp('direct.04')"
				@touchcancel="handleButtonUp('direct.04')">▶</view>
			<view class="circle-btn" @touchstart="handleButtonDown('direct.02')"
				@touchend="handleButtonUp('direct.02')"
				@touchcancel="handleButtonUp('direct.02')">▼</view>
		</view>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				options: {
					deviceId: "",
					serviceId: "",
					characteristicId: ""
				},
				longPressTimer: null
			}
		},
		onLoad(options) {
			this.options = options
		},
		methods: {
			string2Hex(str) {
				let val = ""
				for (let i = 0; i < str.length; i++) {
					if (val == "")
						val = str.charCodeAt(i).toString(16)
					else
						val += str.charCodeAt(i).toString(16)
				}
				return val
			},
			hex2ArrayBuffer(hex_str) {
				let typedArray = new Uint8Array(hex_str.match(/[\da-f]{2}/gi).map(function(h) {
					return parseInt(h, 16)
				}))
				let buffer = typedArray.buffer
				return buffer
			},
			// 写入数据，16进制的
			writeBLECharacteristicValue(val) {
				return new Promise((resolve, reject) => {
					var hex = this.string2Hex(val); //要发送的信息
					var buffer = this.hex2ArrayBuffer(hex)
					console.log("buffer =>", buffer)

					uni.writeBLECharacteristicValue({
						// 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
						deviceId: this.options.deviceId,
						// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
						serviceId: this.options.serviceId,
						// 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
						characteristicId: this.options.characteristicId,
						// 这里的value是ArrayBuffer类型
						value: buffer,
						success(res) {
							console.log('写入成功 success =>', res)
							resolve(res)
						},
						fail(res) {
							console.log("写入失败 =>", res)
							reject(res)
						}
					})
				})
			},
			// 按钮按下
			handleButtonDown(cmdVal) {
				// 长按模拟
				this.longPressTimer = setInterval(async () => {
					console.log("btn interval =>", cmdVal)
					try {
						await this.writeBLECharacteristicValue(cmdVal);
					} catch (e) {}
				}, 100)
			},
			// 按钮抬起
			handleButtonUp(cmdVal) {
				console.log("btn up =>", cmdVal)
				clearInterval(this.longPressTimer)
				this.longPressTimer = null;
			}
		}
	}
</script>
<style>
	.controlPad-container {
		.test {
			width: 150upx;
			height: 90upx;
			border: 1px solid #ccc;
			padding: 5px;
		}

		.direct-container {
			position: relative;
			width: 100%;
			height: auto;

			.circle-btn {
				position: absolute;
				width: 100upx;
				height: 100upx;
				border-radius: 100%;
				border: 1px solid #ccc;
				background-color: #eee;
				text-align: center;
				line-height: 100upx;
				user-select: none;
			}

			.circle-btn:nth-child(1) {
				top: 50upx;
				left: calc(50% - 50upx);
			}

			.circle-btn:nth-child(2) {
				top: 200upx;
				left: calc(150upx);
			}

			.circle-btn:nth-child(3) {
				top: 200upx;
				right: calc(150upx);
			}

			.circle-btn:nth-child(4) {
				top: 350upx;
				left: calc(50% - 50upx);
			}
		}
	}
</style>