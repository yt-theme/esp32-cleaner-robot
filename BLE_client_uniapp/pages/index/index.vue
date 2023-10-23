<template>
	<view class="content">
		<view class="header-area">
			<button type="default" @tap="scan()">{{ isScanning ? '扫描中' : '重新扫描'}}</button>
		</view>

		<!-- 扫描出来的设备列表 -->
		<view class="list">
			<view class="list-one" v-for="(ite,ind) in lists" :key="ite.deviceId" @tap="connect(ite.deviceId)">
				<view :class="['list-one-name', connectSuccess && deviceId === ite.deviceId ? 'list-one-name-act' : '']">
					{{ ite.name }}
				</view>
				<view>
					{{ ite.deviceId }}
				</view>
				<view>
					{{ ite.RSSI }}
				</view>
			</view>
		</view>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				list: [],
				lists: [],
				deviceRecordsObj: {},
				deviceId: '',
				serviceId: '',
				isScanning: false,
				connectSuccess: false,
			}
		},
		onLoad() {
			this.openPhoneBluetooth()
		},
		methods: {
			// 验证是什么平台的，调用打开蓝牙
			openPhoneBluetooth() {
				var main, BluetoothAdapter, BAdapter;
				switch (uni.getSystemInfoSync().platform) {
					case "android":
						// Android平台: plus.android.*  
						console.log("运行在android")
						main = plus.android.runtimeMainActivity();
						BluetoothAdapter = plus.android.importClass("android.bluetooth.BluetoothAdapter");
						BAdapter = BluetoothAdapter.getDefaultAdapter()
						if (!BAdapter.isEnabled()) {
							BAdapter.enable()
						}
						this.scan();
						break;
					case "ios":
						// iOS平台: plus.ios.*  
						console.log("运行在ios")
						break;
					default:
						// 其它平台 
						console.log("运行在其他平台")
						break;
				}
			},

			// 点击扫描设备
			scan() {
				var _this = this
				if (_this.isScanning) {
					return false;
				}
				// 扫描前要断开连接
				this.disconnect()
				
				//在页面加载时候初始化蓝牙适配器
				uni.openBluetoothAdapter({
					success: (res) => { //已打开
						uni.getBluetoothAdapterState({ //蓝牙的匹配状态
							success: (res1) => {
								console.log(res1, '“本机设备的蓝牙已打开”')

								// 开始搜索蓝牙设备
								_this.isScanning = true;
								this.startBluetoothDeviceDiscovery()

							},
							fail(error) {
								uni.showToast({
									icon: 'none',
									title: '查看手机蓝牙是否打开'
								});
							}
						})
					},
					fail: err => { //未打开 
						uni.showToast({
							icon: 'none',
							title: '查看手机蓝牙是否打开'
						});

					}
				})
			},

			// 开始搜索蓝牙设备
			startBluetoothDeviceDiscovery() {
				uni.startBluetoothDevicesDiscovery({
					allowDuplicatesKey: true,
					success: (res) => {
						console.log('startBluetoothDevicesDiscovery success', res)

						// 发现外围设备
						this.onBluetoothDeviceFound()
					},
					fail: err => {
						console.log(err, '错误信息')
					}
				})
			},

			// 发现外围设备
			onBluetoothDeviceFound() {
				console.log("发现外围设备ing")
				uni.onBluetoothDeviceFound((res) => {
					// console.log(res)
					// console.log(res.devices)
					// console.log(res.devices[0])
					// console.log(res.devices[0].name)
					// if(this.list.indexOf(res.devices[0].deviceId)==-1){
					console.log(res.devices)
					// this.list.push(res.devices[0].deviceId)
					// if(res.devices[0].name!=""){
					// if(res.devices[0].name.indexOf("Scooter") != -1){
					// this.lists.push([
					// 	res.devices[0].name,
					// 	res.devices[0].deviceId,
					// 	res.devices[0].RSSI
					// ])
					// }

					// }
					// console.log(res.devices[0].name)
					// console.log(this.lists)

					// }

					this.deviceRecordsObj[res.devices[0].deviceId] = res.devices[0]
					this.lists = [];
					for (let key in this.deviceRecordsObj) {
						this.lists.push({
							...this.deviceRecordsObj[key],
							name: this.deviceRecordsObj[key].name,
							deviceId: this.deviceRecordsObj[key].deviceId,
							RSSI: this.deviceRecordsObj[key].RSSI
						})
					}

				})
			},

			//点击连接
			async connect(deviceId) {
				let thit = this
				
				// 如果已有连接则先断开连接
				if (this.connectSuccess && this.deviceId) {
					try {
						await this.disconnect();
					} catch (e) {
						uni.showToast({
							title: '已有连接断开失败'
						});
						return false;
					}
				}

				//连接蓝牙
				uni.createBLEConnection({
					// 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
					// 传入res.devices[0].deviceId
					deviceId: deviceId,
					async success(res) {
						await thit.stopBluetoothDevicesDiscovery()
						uni.showToast({
							title: '蓝牙连接成功'
						});
						thit.deviceId = deviceId;
						thit.connectSuccess = true;

						thit.getBLEDeviceServices();

					},
					fail(res) {
						console.log("错误", res)
						console.log("蓝牙连接失败")
						thit.connectSuccess = false;
					}
				})

			},
			// 停止搜寻蓝牙设备
			stopBluetoothDevicesDiscovery() {
				return new Promise((resolve, reject) => {
					uni.stopBluetoothDevicesDiscovery({
						success: e => {
							console.log("断开成功")
							console.log('停止搜索蓝牙设备:' + e.errMsg);
							this.isScanning = false;
							resolve(e)
						},
						fail: e => {
							console.log("断开失败")
							console.log('停止搜索蓝牙设备失败，错误码：' + e.errCode);
							reject(e)
						}
					});
				})
			},

			//获取蓝牙的所有服务
			getBLEDeviceServices() {
				setTimeout(() => {
					uni.getBLEDeviceServices({
						// 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
						deviceId: this.deviceId,
						success: (res) => {
							//res.services[1].uuid
							console.log("成功", res)
							console.log('device services:', res)
							console.log(res.services)
							let uuid = res.services.map(i => i.uuid)
							console.log(uuid)

							// if(!uuid.includes("00008880-7777-EFDE-1523-785FEABCD123")){
							// 	uni.showToast({
							// 		icon:"none",
							// 		title: '请连接对应的设备'
							// 	});
							// 	this.disconnect();
							// }
							res.services.forEach((item) => {
								console.log("services item =>", item)
								if (item.uuid === "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
									.toUpperCase()) {
									this.serviceId = item.uuid
									console.log(this.serviceId)
									// 必须在这里的回调才能获取
									uni.onBLECharacteristicValueChange((res) => {
										console.log("监听成功", res)

									})
									//获取意外断开蓝牙
									uni.onBLEConnectionStateChange((res) => {
										// 该方法回调中可以用于处理连接意外断开等异常情况
										console.log("获取意外断开蓝牙 =>", res)
										if (res.connected == false) {
											clearInterval(this.time)
											uni.showToast({
												icon: "none",
												title: '蓝牙连接已断开'
											});
											this.connectSuccess = false;
										}

										console.log(
											`device ${res.deviceId} state has changed, connected: ${res.connected}`
											)
									})

									this.getBLEDeviceCharacteristics()
								}
							})
						}
					})
				}, 500)
			},

			//获取蓝牙特征
			getBLEDeviceCharacteristics() {
				console.log("进入特征");
				setTimeout(() => {
					uni.getBLEDeviceCharacteristics({
						// 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
						deviceId: this.deviceId,
						// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
						serviceId: this.serviceId,
						success: (res) => {
							console.log(res, 'getBLEDeviceCharacteristics')
							this.characteristics = res.characteristics
							// this.$store.commit("chara",res.characteristics)
							// console.log(this.characteristics)
							// console.log(this.characteristics[0].uuid)

							res.characteristics.forEach((item) => {
								if (item.uuid === "beb5483e-36e1-4688-b7f5-ea07361b26a8"
									.toUpperCase()) {
									console.log('characteristicId item.uuid => ' + item.uuid)
									// this.notifyBLECharacteristicValueChange(item.uuid)
									// 进入操作页面
									uni.navigateTo({
										url: `/pages/controlPad/index?deviceId=${this.deviceId}&serviceId=${this.serviceId}&characteristicId=${item.uuid}`,
									})
								}
							})

						},
						fail: (err) => {
							console.log("getBLEDeviceCharacteristics err =>", err)
						}
					})
				}, 100)
			},

			// 启用 notify 功能
			notifyBLECharacteristicValueChange(characteristicId) {
				console.log("启用 notify 功能 =>", characteristicId)
				// console.log(characteristicId,'characteristicId')
				uni.notifyBLECharacteristicValueChange({
					state: true, // 启用 notify 功能
					// 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
					deviceId: this.deviceId,
					// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
					serviceId: this.serviceId,
					// 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
					characteristicId: characteristicId,
					success: (res) => {
						console.log("开启成功了，准备开始读取", res)

						clearInterval(this.time)
						this.time = setInterval(() => {
							uni.readBLECharacteristicValue({
								// 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
								deviceId: this.deviceId,
								// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
								serviceId: this.serviceId,
								// 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
								characteristicId: characteristicId,
								success: (res) => {
									console.log("读取成功222")
									// console.log("这里读到的蓝牙uuid是"+characteristicId)

									// console.log('readBLECharacteristicValue:', res)
								},
								fail: (res) => {
									console.log("读取失败" + characteristicId)
									console.log('readBLECharacteristicValue:', res)
								}
							})
						}, 1000)
					},
					fail: (err) => {
						console.log('notifyBLECharacteristicValueChange err', err)
					}
				})

			},

			// 写入数据，16进制的
			writeBLECharacteristicValue(e) {
				let thit = this
				let stconf = "000"
				var hex = stconf.join("")
				console.log(hex)
				var typedArray = new Uint8Array(hex.match(/[\da-f]{2}/gi).map(function(h) {
					return parseInt(h, 16)
				}))

				var buffer = typedArray.buffer
				uni.writeBLECharacteristicValue({
					// 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
					deviceId: this.deviceId,
					// 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
					serviceId: this.serviceId,
					// 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
					characteristicId: this.characteristics[0].uuid,
					// 这里的value是ArrayBuffer类型
					value: buffer,
					success(res) {
						console.log('writeBLECharacteristicValue success', res.errMsg)
						console.log("写入成功")
					},
					fail(res) {
						console.log("写入失败", res)
						// 重新写入一次
						setTimeout(() => {
							thit.writeBLECharacteristicValue(e)
						}, 100)
					}
				})
			},

			//断开蓝牙连接
			disconnect() {
				return new Promise((resolve, reject) => {
					uni.closeBLEConnection({
						deviceId: this.deviceId,
						success(res) {
							console.log(res)
							resolve(res)
						},
						fail() {
							reject()
						}
					})
				})
			},
		}
	}
</script>

<style>
	.content {
		position: relative;
		width: 100%;
		height: auto;
	}

	.header-area {
		position: fixed;
		top: 0;
		left: 0;
		width: 100%;
		height: 155upx;
		background-color: #f4f4f4;
	}

	.list {
		height: auto;
		overflow-y: scroll;
		background: #fff;
		border-radius: 10rpx;
		margin: 20rpx;
		margin-top: 155upx;

		.list-one {
			display: flex;
			justify-content: space-between;
			align-items: center;
			border-bottom: 1px solid #E6E6E6;
			height: 105rpx;
			line-height: 105rpx;
			font-size: 30rpx;
			color: #333333;
			padding: 0 28rpx;

			.list-one-name {
				padding: 3px 5px;
				background-color: aquamarine;
				border-radius: 3upx;
			}
			.list-one-name-act {
				background-color: coral !important;
			}
		}

		.list-one:last-child {
			border: none;
		}

		.list-one:active {
			background: #CCCCCC;
		}
	}
</style>