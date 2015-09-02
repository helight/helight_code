package main

import (
	"code.google.com/p/go.crypto/ssh"
	"io"
	"log"
)

var (
	pemBytes = `-----BEGIN RSA PRIVATE KEY-----
    .......
    .......
    .......
    -----END RSA PRIVATE KEY-----`
)

type keychain struct {
	signer ssh.Signer
}

func (k *keychain) Key(i int) (ssh.PublicKey, error) {
	if i != 0 {
		return nil, nil
	}
	pk := k.signer.PublicKey()
	return pk, nil
}

func (k *keychain) Sign(i int, rand io.Reader, data []byte) (sig *ssh.Signature, err error) {
	return k.signer.Sign(rand, data)
}

func main() {
	// signer, err := ssh.ParsePrivateKey([]byte(pemBytes))
	// if err != nil {
	// 	panic(err)
	// }

	// clientKey := &keychain{signer}

	password := "helightxu"
	authMethods := []ssh.AuthMethod{}
	keyboardInteractiveChallenge := func(
		user,
		instruction string,
		questions []string,
		echos []bool,
	) (answers []string, err error) {
		if len(questions) == 0 {
			return []string{}, nil
		}
		return []string{password}, nil
	}
	authMethods = append(authMethods, ssh.KeyboardInteractive(keyboardInteractiveChallenge))
	authMethods = append(authMethods, ssh.Password(password))
	config := &ssh.ClientConfig{
		User: "helight",
		Auth: authMethods,
	}

	c, err := ssh.Dial("tcp", "127.0.0.1:22", config)
	if err != nil {
		log.Println("unable to dial remote side:", err)
	}
	defer c.Close()

	// Create a session
	session, err := c.NewSession()
	if err != nil {
		log.Fatalf("unable to create session: %s", err)
	}
	defer session.Close()

	b, err := session.Output("ls /data/ -l")
	if err != nil {
		log.Fatalf("failed to execute: %s", err)
	}
	log.Println("Output: ", string(b))

	return
}
