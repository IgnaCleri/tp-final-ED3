## Git Hooks

Este repositorio versiona sus hooks locales dentro de `.githooks/`.

Para activarlos en tu clon:

```bash
git config core.hooksPath .githooks
```

Actualmente se incluye:

- `pre-commit`: bloquea whitespace roto y evita commitear carpetas locales del IDE o artefactos generados de build.
