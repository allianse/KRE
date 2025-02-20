// Copyright (c) 2022 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Module for [`ChronikServer`].

use std::net::SocketAddr;

use abc_rust_error::Result;
use axum::Router;
use hyper::server::conn::AddrIncoming;
use thiserror::Error;

/// Params defining what and where to serve for [`ChronikServer`].
#[derive(Clone, Debug)]
pub struct ChronikServerParams {
    /// Host address (port + IP) where to serve Chronik at.
    pub hosts: Vec<SocketAddr>,
}

/// Chronik HTTP server, holding all the data/handles required to serve an
/// instance.
#[derive(Debug)]
pub struct ChronikServer {
    server_builders: Vec<hyper::server::Builder<AddrIncoming>>,
}

/// Errors for [`BlockWriter`] and [`BlockReader`].
#[derive(Debug, Eq, Error, PartialEq)]
pub enum ChronikServerError {
    /// Binding to host address failed
    #[error("Chronik failed binding to {0}: {1}")]
    FailedBindingAddress(SocketAddr, String),

    /// Serving Chronik failed
    #[error("Chronik failed serving: {0}")]
    ServingFailed(String),
}

use self::ChronikServerError::*;

impl ChronikServer {
    /// Binds the Chronik server on the given hosts
    pub fn setup(params: ChronikServerParams) -> Result<Self> {
        let server_builders = params
            .hosts
            .into_iter()
            .map(|host| {
                axum::Server::try_bind(&host).map_err(|err| {
                    FailedBindingAddress(host, err.to_string()).into()
                })
            })
            .collect::<Result<Vec<_>>>()?;
        Ok(ChronikServer { server_builders })
    }

    /// Serve a Chronik HTTP endpoint with the given parameters.
    pub async fn serve(self) -> Result<()> {
        let app = Router::new();
        let servers = self
            .server_builders
            .into_iter()
            .zip(std::iter::repeat(app))
            .map(|(server_builder, app)| {
                Box::pin(async move {
                    server_builder
                        .serve(app.into_make_service())
                        .await
                        .map_err(|err| ServingFailed(err.to_string()))
                })
            });
        let (result, _, _) = futures::future::select_all(servers).await;
        result?;
        Ok(())
    }
}
