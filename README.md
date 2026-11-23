# onnxruntime-builds

Static builds of [ONNX Runtime](https://onnxruntime.ai) for the targets Octomind ships, published as release assets.

The upstream repository `github.com/microsoft/onnxruntime` has returned 404 since 2026-11-17, and no prebuilt static library exists for musl Linux. This repository builds the library from one pinned commit and keeps a copy of that source, so a release does not depend on a third-party host being up.

## What a release contains

Each release is tagged `v<onnxruntime version>` and holds:

| Asset | Contents |
|---|---|
| `onnxruntime-<version>-<target>.tar.gz` | One merged static library (`libonnxruntime.a`, or `onnxruntime.lib` on Windows), `LICENSE`, `ThirdPartyNotices.txt` |
| `onnxruntime-<version>-src.tar.gz` | The source tree the libraries were built from, submodules included |
| `<asset>.sha256` | SHA-256 of the asset, in `shasum -c` format |

Targets:

- `x86_64-unknown-linux-musl`
- `aarch64-unknown-linux-musl`
- `aarch64-apple-darwin` (with the CoreML execution provider)
- `x86_64-pc-windows-msvc`
- `aarch64-pc-windows-msvc`

The musl libraries are built on Alpine 3.22. Link them in an Alpine 3.22 image too, because the archive expects that release's C++ runtime. The Windows libraries use the dynamic C runtime (`/MD`).

## How the source is verified

The build clones a mirror and stops unless the checkout is the commit pinned in `.github/workflows/build.yml`. For 1.24.2 that is `058787ceead760166e3c50a0a4cba8a833a6f53f`. Two independent sources confirm it:

- the official PyPI `onnxruntime` 1.24.2 wheel reports `git-commit-id=058787ceea`;
- [Software Heritage](https://archive.softwareheritage.org/api/1/revision/058787ceead760166e3c50a0a4cba8a833a6f53f/) archived the same commit from the original repository, with tree `e42d56bb417bdecf5e1f4f27a75ce01709acb302`.

## Using a build with the `ort` crate

`ort-sys` links a single `libonnxruntime.a` when `ORT_LIB_LOCATION` points at the directory that holds it:

```sh
asset=onnxruntime-1.24.2-x86_64-unknown-linux-musl.tar.gz
base=https://github.com/Muvon/onnxruntime-builds/releases/download/v1.24.2
curl -fsSL -O "$base/$asset" -O "$base/$asset.sha256"
shasum -a 256 -c "$asset.sha256"
mkdir onnxruntime && tar -xzf "$asset" -C onnxruntime
ORT_LIB_LOCATION="$PWD/onnxruntime" cargo build --release --target x86_64-unknown-linux-musl
```

## Building another version

Change `ORT_VERSION` and `ORT_COMMIT` in `.github/workflows/build.yml` and push to `master`. The workflow builds every target and publishes the release for that version. A target that fails does not block the others.

ONNX Runtime is MIT licensed; each archive carries its license and third-party notices.
