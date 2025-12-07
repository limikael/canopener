int main() {
	SlcanStdioBus bus;
	Device device(bus);

	while (1) {
		device.loop();
	}

	return 0;
}
