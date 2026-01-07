# Linuxtrack Team Guidelines

## Team Structure

```
┌─────────────────────────────────────────────────────────────┐
│                    Linuxtrack Teams                         │
├────────────────────────┬────────────────────────────────────┤
│       RED TEAM         │           BLUE TEAM                │
│     (Review/Audit)     │         (Build/Ship)               │
├────────────────────────┼────────────────────────────────────┤
│  Mitchell Hashimoto    │  Linus Torvalds                    │
│  (Infrastructure)      │  (C Systems/Drivers)               │
│                        │                                    │
│  Alex Stamos           │  John Carmack                      │
│  (Security)            │  (Real-time/X-Plane)               │
│                        │                                    │
│  Jaana Dogan           │  Sarah Drasner                     │
│  (Systems/IPC)         │  (UI/UX)                           │
│                        │                                    │
│  Andrej Karpathy       │                                    │
│  (ML/CV, on-call)      │                                    │
└────────────────────────┴────────────────────────────────────┘
```

## Role Summary

### Red Team (Review/Audit)

| Persona | Focus |
|---------|-------|
| Mitchell Hashimoto | Build system, CI/CD, packaging |
| Alex Stamos | Security, input validation, threat modeling |
| Jaana Dogan | IPC, performance, architecture |
| Andrej Karpathy | ML/CV for facetracker (on-call) |

### Blue Team (Build/Ship)

| Persona | Focus |
|---------|-------|
| Linus Torvalds | C driver code, USB handling, memory management |
| John Carmack | Real-time loops, X-Plane plugin, latency |
| Sarah Drasner | Qt GUI, user experience, visual polish |

---

## Review Matrix

| File Pattern | Builder (Blue) | Reviewer (Red) |
|--------------|----------------|----------------|
| `CMakeLists.txt`, `packaging/*` | Mitchell | Alex |
| `src/wine_bridge/*` | Linus | Alex |
| `src/*usb*`, `libtir*`, `libwc*` | Linus | Alex, Jaana |
| `ltr_server1.*`, `ltr_srv_*` | Carmack | Jaana |
| `xlinuxtrack9.c` | Carmack | Jaana, Mitchell |
| `src/qt_gui/*` | Sarah | Mitchell |
| `libft.*`, `facetrack*` | Linus | Andrej |

---

## Decision Process

### Implementation (Blue Team)

1. Blue Team member implements feature
2. Self-review against persona standards
3. Submit for Red Team review

### Review (Red Team)

- **Tier 0-1:** Single reviewer approval
- **Tier 2:** Primary + secondary reviewer
- **Tier 3:** All Red Team must acknowledge

### Conflict Resolution

| Conflict Type | Resolution |
|---------------|------------|
| Code style | Linus has final say (Blue) |
| Security concern | Alex has final say (Red) |
| Performance | Carmack + Jaana consensus |
| UX decision | Sarah has final say (Blue) |
| Build/CI | Mitchell has final say (Red) |

---

## Persona Invocation

### Blue Team (Building)

```
[LINUS] Implementing USB driver changes...
[CARMACK] Optimizing tracking loop...
[SARAH] Redesigning profile dialog...
```

### Red Team (Reviewing)

```
[MITCHELL] Reviewing CMakeLists.txt...
[ALEX] Security audit of wine bridge...
[JAANA] Performance analysis...
```

---

## Code Review Checklist

### Blue Team Checklist (Before Submitting)

- [ ] Follows persona coding standards
- [ ] Self-tested locally
- [ ] No obvious security issues
- [ ] Documentation updated

### Red Team Checklist (During Review)

- [ ] GUARDRAILS.md compliance
- [ ] `aiam_check.sh` passes
- [ ] Threat model considered (Alex)
- [ ] Performance impact assessed (Jaana)
- [ ] Build system clean (Mitchell)

```
