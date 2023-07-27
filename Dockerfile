FROM alpine:latest as builder

RUN apk update && apk add --no-cache build-base musl-dev clang

COPY . .

RUN make all

RUN mkdir exports
RUN cp build/server exports/ 

FROM scratch

COPY --from=builder exports/server /

EXPOSE 8080

ENTRYPOINT [ "/server" ]