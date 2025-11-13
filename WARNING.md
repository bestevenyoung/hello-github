# Godnux Kernel Warnings and Known Issues

Godnux is under active development. While the kernel core and module infrastructure are functional, there are **known issues and limitations** that all users and contributors should be aware of before deploying or extending the system.

---

## ⚠️ Stability Concerns
- **Kernel Panics**: Like Linux, Godnux can hit unrecoverable faults that trigger a panic. This halts the system immediately to protect data, but results in downtime until a restart.
- **Boot Failures**: Custom builds may fail to boot due to misconfigured init routines or missing modules. Blank screens and GRUB-like loader errors are possible.
- **Scheduling Bugs**: High-priority scheduling requests may bypass privilege checks in development mode, leading to unfair CPU allocation.

---

## ⚠️ Security Limitations
- **Trust Enforcement Disabled**: Currently, all modules are treated as trusted. Capability-based restrictions are not enforced, meaning malicious or buggy modules can run unchecked.
- **Unsigned Modules**: Secure Boot–style enforcement is not yet implemented. Unsigned or tampered `.so` files may still load.
- **Privilege Escalation Risks**: In development mode, user-level processes may attempt to create higher-privilege processes. While blocked in principle, enforcement is incomplete.

---

## ⚠️ Compatibility Issues
- **Hardware Support**: Limited testing across diverse hardware. Some devices may fail to initialize correctly, similar to Linux kernel hardware compatibility problems.
- **Virtualization Layers**: Running inside VMs may expose timing and resource bugs not seen on bare metal.
- **Module API Volatility**: The module interface is evolving. Functions and signatures may change without backward compatibility guarantees.

---

## ⚠️ Development Mode Caveats
- **Audit Logging Incomplete**: Security events are not fully logged or persisted.
- **Memory Manager Edge Cases**: Fragmentation and allocation tracking may report incorrect free/used totals under stress.
- **Process Cleanup**: Destroyed processes may leave dangling references in the registry.

---

## 🚨 Summary
Godnux is **experimental software**. It is not production-ready and should only be used for development, testing, and research. Expect instability, incomplete security enforcement, and evolving APIs.  

By using Godnux, you acknowledge these risks and agree to operate in a **development environment only** until stability and security milestones are reached.

---

## References
- Common Linux boot errors  
- Kernel panic causes and troubleshooting  
- General kernel issues and compatibility problems

