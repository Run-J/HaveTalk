# =======================================================
#                 Can We Talk Makefile
# =======================================================


# Subdir path
Common_DIR = common
Server_DIR = chat-server
Client_DIR = chat-client


# =======================================================
#                    Build All
# =======================================================


all:
	@echo "SetUp Server..."
	$(MAKE) -C $(Server_DIR)

	@echo ""
	@echo "SetUp Client..."
	$(MAKE) -C $(Client_DIR)

	@echo ""
	@echo "Copying chat-server and chat-client to same directory (Common/bin)..."
	cp $(Server_DIR)/bin/chat-server ./common/bin/
	cp $(Client_DIR)/bin/chat-client ./common/bin/


# =======================================================
#                   Clean All
# =======================================================
clean:
	$(MAKE) -C $(Server_DIR) clean
	$(MAKE) -C $(Client_DIR) clean
	$(MAKE) -C $(Common_DIR) clean
