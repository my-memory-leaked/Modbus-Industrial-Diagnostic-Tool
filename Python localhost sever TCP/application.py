import sys
import threading
import tkinter as tk
from tkinter import scrolledtext
from pymodbus.server.sync import ModbusTcpServer
from pymodbus.device import ModbusDeviceIdentification
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext

# Function to run the Modbus server
def run_server(app, stop_event):
    store = ModbusSequentialDataBlock(0, [0] * 100)
    context = ModbusSlaveContext(di=store, co=store, hr=store, ir=store)
    context = ModbusServerContext(slaves=context, single=True)

    identity = ModbusDeviceIdentification()
    identity.VendorName = 'Pymodbus'
    identity.ProductCode = 'PM'
    identity.VendorUrl = 'http://github.com/riptideio/pymodbus/'
    identity.ProductName = 'Pymodbus Server'
    identity.ModelName = 'Pymodbus Server'
    identity.MajorMinorRevision = '2.3.0'

    server = ModbusTcpServer(context, identity=identity, address=("localhost", 5020))

    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.start()

    # Wait for the stop event
    stop_event.wait()

    server.shutdown()  # Shutdown the server
    server.server_close()  # Close the server
    app.write_to_console("Server stopped.")

# GUI Application
class ModbusServerApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Modbus TCP Server")
        self.geometry("800x600")

        self.start_button = tk.Button(self, text="Start Server", command=self.start_server)
        self.start_button.pack(pady=10)

        self.stop_button = tk.Button(self, text="Stop Server", command=self.stop_server, state=tk.DISABLED)
        self.stop_button.pack(pady=10)

        self.console = scrolledtext.ScrolledText(self, state='disabled', height=10)
        self.console.pack(pady=10)

        self.server_thread = None
        self.stop_event = threading.Event()

    def write_to_console(self, text):
        self.console.configure(state='normal')
        self.console.insert(tk.END, text + '\n')
        self.console.configure(state='disabled')

    def start_server(self):
        self.write_to_console("Starting server...")
        self.stop_event.clear()
        self.server_thread = threading.Thread(target=run_server, args=(self, self.stop_event), daemon=True)
        self.server_thread.start()
        self.start_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)

    def stop_server(self):
        self.write_to_console("Stopping server...")
        self.stop_event.set()  # Signal the server thread to stop
        # Do not join the server thread here, as it will block the main thread
        self.start_button.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)

if __name__ == "__main__":
    app = ModbusServerApp()
    app.mainloop()
